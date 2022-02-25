/* SPDX-License-Identifier: GPL-2.0-or-later */

/* DMA Buffer Exporter Kernel Mode Driver. 
 * Copyright (C) 2021 Intel Corporation.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */
 
#include <linux/device.h>
#include <linux/dma-buf.h>
#include <linux/highmem.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/miscdevice.h>
#include "./dma_buf_exporter_kmd.h"

MODULE_IMPORT_NS(DMA_BUF);

#define DRIVER_VERSION  "1.0.0-0"
#define DRIVER_AUTHOR   "Intel Corporation"
#define DRIVER_DESC     "DMA Buffer Exporter Driver"

static struct dma_buf *dma_buf_exporter_alloc(size_t size);
static void dma_buf_exporter_free(struct dma_buf *dma_buf);


static int dma_buf_exporter_attach(struct dma_buf *dmabuf, struct dma_buf_attachment *attachment)
{
	return 0;
}

static void dma_buf_exporter_detach(struct dma_buf *dmabuf, struct dma_buf_attachment *attachment)
{
	return;
}

static struct sg_table *dma_buf_exporter_map_dma_buf(struct dma_buf_attachment *attachment,
					 enum dma_data_direction dir)
{
	struct sg_table *table = NULL;

	return table;
}

static void dma_buf_exporter_unmap_dma_buf(struct dma_buf_attachment *attachment,
			       struct sg_table *table,
			       enum dma_data_direction dir)
{
	return;
}

static const struct dma_buf_ops dma_buf_exporter_ops = {
	.attach = dma_buf_exporter_attach,
	.detach = dma_buf_exporter_detach,
	.map_dma_buf = dma_buf_exporter_map_dma_buf,
	.unmap_dma_buf = dma_buf_exporter_unmap_dma_buf,
};

struct dma_buf_exporter_data {
	int num_pages;
	struct page *pages[];
};

static struct dma_buf *dma_buf_exporter_alloc(size_t size)
{
	DEFINE_DMA_BUF_EXPORT_INFO(dma_buf_exporter_info);
	struct dma_buf *dma_buf;
	struct dma_buf_exporter_data *data;
	int i, npages;

	npages = PAGE_ALIGN(size) / PAGE_SIZE;
	if (!npages)
		return ERR_PTR(-EINVAL);

	data = kmalloc(sizeof(*data) + npages * sizeof(struct page *),
		       GFP_KERNEL);
	if (!data)
		return ERR_PTR(-ENOMEM);

	for (i = 0; i < npages; i++) {
		data->pages[i] = alloc_page(GFP_KERNEL);
		if (!data->pages[i])
			goto err;
	}
	data->num_pages = npages;

	dma_buf_exporter_info.ops = &dma_buf_exporter_ops;
	dma_buf_exporter_info.size = npages * PAGE_SIZE;
	dma_buf_exporter_info.flags = O_CLOEXEC;
	dma_buf_exporter_info.priv = data;

	dma_buf = dma_buf_export(&dma_buf_exporter_info);
	if (IS_ERR(dma_buf))
		goto err;

	return dma_buf;

err:
	while (i--)
		put_page(data->pages[i]);
	kfree(data);
	return ERR_PTR(-ENOMEM);
}

static void dma_buf_exporter_free(struct dma_buf *dma_buf)
{
	struct dma_buf_exporter_data *data = dma_buf->priv;
	int i;

	for (i = 0; i < data->num_pages; i++)
		put_page(data->pages[i]);

	kfree(data);
}

static long dma_buf_exporter_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	struct dma_exporter_buf_alloc_data data;
	struct dma_buf *dma_buf;

	if (copy_from_user(&data, (void __user *)arg, sizeof(data))) {
		pr_info("dma_buf_exporter: failed to copy user data. ");
		return -EFAULT;
	}

	switch (cmd) {
		case DMA_BUF_EXPORTER_ALLOC: {
			
			pr_info("dma_buf_exporter: allocating dma_buf of size %llu ", data.size);
			dma_buf = dma_buf_exporter_alloc(data.size);
			if (!dma_buf) {
				pr_err("error: exporter alloc page failed\n");
				return -ENOMEM;
			}
			
			data.fd = dma_buf_fd(dma_buf, O_CLOEXEC);

			if (copy_to_user((void __user *)arg, &data, sizeof(data)))
				return -EFAULT;

			return 0;
		}

		case DMA_BUF_EXPORTER_FREE: {
				pr_info("dma_buf_exporter: freeing dma_buf of size %llu ", data.size);
				dma_buf = dma_buf_get(data.fd);
				
				if (IS_ERR(dma_buf))
					return PTR_ERR(dma_buf);

				dma_buf_exporter_free(dma_buf);
				return 0;
		}

		default: {
			pr_info("dma_buf_exporter: invalid IOCTL code %u ", cmd);
			return -EINVAL;
		}

	}

	return 0;
}
 
static struct file_operations dma_buf_exporter_fops = {
	.owner   = THIS_MODULE,
	.unlocked_ioctl   = dma_buf_exporter_ioctl,
};
 
static struct miscdevice dma_buf_exporter_dev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = DMA_BUF_EXPORTER_DEV_NAME,
	.fops = &dma_buf_exporter_fops,
};
 
static int __init dma_buf_exporter_init(void)
{
	pr_info("Loading dma_buf_exporter_kmd ...");
	return misc_register(&dma_buf_exporter_dev);
}

static void __exit dma_buf_exporter_exit(void)
{
	pr_info("Unloading dma_buf_exporter_kmd ...");
	misc_deregister(&dma_buf_exporter_dev);
}


module_init(dma_buf_exporter_init);
module_exit(dma_buf_exporter_exit);

MODULE_VERSION(DRIVER_VERSION);
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_LICENSE("GPL v2");
