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


#define DRIVER_VERSION  "1.0.0-0"
#define DRIVER_AUTHOR   "Intel Corporation"
#define DRIVER_DESC     "DMA Buffer Exporter Driver"


#if 0
static int dma_buf_exporter_attach(struct dma_buf *dmabuf, struct device *dev,
			struct dma_buf_attachment *attachment)
{
	return 0;
}

static void dma_buf_exporter_detach(struct dma_buf *dmabuf, struct dma_buf_attachment *attachment)
{
	return;
}

static void dma_buf_exporter_release(struct dma_buf *dma_buf)
{
	return;
}

static void *dma_buf_exporter_vmap(struct dma_buf *dma_buf)
{
	return NULL;
}

static void dma_buf_exporter_vunmap(struct dma_buf *dma_buf, void *vaddr)
{
	return;
}

static void *dma_buf_exporter_kmap_atomic(struct dma_buf *dma_buf, unsigned long page_num)
{
	return NULL;
}

static void dma_buf_exporter_kunmap_atomic(struct dma_buf *dma_buf, unsigned long page_num, void *addr)
{
	return;
}

static void *dma_buf_exporter_kmap(struct dma_buf *dma_buf, unsigned long page_num)
{

	return NULL;
}

static void dma_buf_exporter_kunmap(struct dma_buf *dma_buf, unsigned long page_num, void *addr)
{
	return ;
}

static int dma_buf_exporter_mmap(struct dma_buf *dma_buf, struct vm_area_struct *vma)
{

	return 0;
}

static int dma_buf_exporter_begin_cpu_access(struct dma_buf *dmabuf,
				      enum dma_data_direction dir)
{
	return 0;
}

static int dma_buf_exporter_end_cpu_access(struct dma_buf *dmabuf,
				enum dma_data_direction dir)
{
		return 0;
}

#endif

static struct sg_table *dma_buf_exporter_map_dma_buf(struct dma_buf_attachment *attachment,
					 enum dma_data_direction dir)
{
	struct sg_table *table;

	return table;
}

static void dma_buf_exporter_unmap_dma_buf(struct dma_buf_attachment *attachment,
			       struct sg_table *table,
			       enum dma_data_direction dir)
{
	return;
}

static const struct dma_buf_ops exp_dmabuf_ops = {
	//.attach = dma_buf_exporter_attach,
	//.detach = dma_buf_exporter_detach,
	.map_dma_buf = dma_buf_exporter_map_dma_buf,
	.unmap_dma_buf = dma_buf_exporter_unmap_dma_buf,
	//.release = dma_buf_exporter_release,
	//.map = dma_buf_exporter_kmap,
	//.unmap = dma_buf_exporter_kunmap,
	//.map_atomic = dma_buf_exporter_kmap_atomic,
	//.unmap_atomic = dma_buf_exporter_kunmap_atomic,
	//.mmap = dma_buf_exporter_mmap,
	//.vmap = dma_buf_exporter_vmap,
	//.vunmap = dma_buf_exporter_vunmap,
	//.begin_cpu_access = dma_buf_exporter_begin_cpu_access,
	//.end_cpu_access = dma_buf_exporter_end_cpu_access,
};

static long dma_buf_exporter_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{

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
	return misc_register(&dma_buf_exporter_dev);
}

static void __exit dma_buf_exporter_exit(void)
{
	misc_deregister(&dma_buf_exporter_dev);
}

module_init(dma_buf_exporter_init);
module_exit(dma_buf_exporter_exit);

MODULE_VERSION(DRIVER_VERSION);
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_LICENSE("GPL v2");
