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
 
#ifndef _UAPI_LINUX_DMA_BUF_EXPORTER__H
#define _UAPI_LINUX_DMA_BUF_EXPORTER__H
#include <linux/ioctl.h>
#include <linux/types.h>

#define DMA_BUF_EXPORTER_DEV_NAME "/dev/dma_buf_exporter"

struct dma_exporter_buf_alloc_data {
	__u32 fd; /* dma_buf_fd */
	__u64 size; /* size in bytes */
	__u32 reserved0;
	__u32 reserved1;
	__u32 reserved2;
};

#define DMA_BUF_EXPORTER_MAGIC		'D'

#define DMA_BUF_EXPORTER_ALLOC		_IOWR(DMA_BUF_EXPORTER_MAGIC, 0, \
				      struct dma_exporter_buf_alloc_data)
#define DMA_BUF_EXPORTER_FREE     _IOWR(DMA_BUF_EXPORTER_MAGIC, 8, \
					struct dma_exporter_buf_alloc_data)
                    
#endif /* _UAPI_LINUX_DMA_BUF_EXPORTER__H */