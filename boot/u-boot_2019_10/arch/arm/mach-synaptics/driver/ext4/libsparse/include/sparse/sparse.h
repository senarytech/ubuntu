/*
 * Copyright (C) 2012 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _LIBSPARSE_SPARSE_H_
#define _LIBSPARSE_SPARSE_H_

#include <common.h>
#include <blk.h>

#ifdef	__cplusplus
extern "C" {
#endif

struct sparse_file;

/**
 * sparse_file_new - create a new sparse file cookie
 *
 * @block_size - minimum size of a chunk
 * @len - size of the expanded sparse file.
 *
 * Creates a new sparse_file cookie that can be used to associate data
 * blocks.  Can later be written to a file with a variety of options.
 * block_size specifies the minimum size of a chunk in the file.  The maximum
 * size of the file is 2**32 * block_size (16TB for 4k block size).
 *
 * Returns the sparse file cookie, or NULL on error.
 */
struct sparse_file *sparse_file_new(unsigned int block_size, int64_t len);

/**
 * sparse_file_destroy - destroy a sparse file cookie
 *
 * @s - sparse file cookie
 *
 * Destroys a sparse file cookie.  After destroy, all memory passed in to
 * sparse_file_add_data can be freed by the caller
 */
void sparse_file_destroy(struct sparse_file *s);

/**
 * sparse_file_add_data - associate a data chunk with a sparse file
 *
 * @s - sparse file cookie
 * @data - pointer to data block
 * @len - length of the data block
 * @block - offset in blocks into the sparse file to place the data chunk
 *
 * Associates a data chunk with a sparse file cookie.  The region
 * [block * block_size : block * block_size + len) must not already be used in
 * the sparse file. If len is not a multiple of the block size the data
 * will be padded with zeros.
 *
 * The data pointer must remain valid until the sparse file is closed or the
 * data block is removed from the sparse file.
 *
 * Returns 0 on success, negative errno on error.
 */
int sparse_file_add_data(struct sparse_file *s,
		void *data, int flag, unsigned int len, unsigned int block);


/**
 * sparse_file_write - write a sparse file to a file
 *
 * @s - sparse file cookie
 * @fd - file descriptor to write to
 * @gz - write a gzipped file
 * @sparse - write in the Android sparse file format
 * @crc - append a crc chunk
 *
 * Writes a sparse file to a file.  If gz is true, the data will be passed
 * through zlib.  If sparse is true, the file will be written in the Android
 * sparse file format.  If sparse is false, the file will be written by seeking
 * over unused chunks, producing a smaller file if the filesystem supports
 * sparse files.  If crc is true, the crc of the expanded data will be
 * calculated and appended in a crc chunk.
 *
 * Returns 0 on success, negative errno on error.
 */
int sparse_file_write(struct sparse_file *s, struct blk_desc *dev,
		lbaint_t start);


#ifdef	__cplusplus
}
#endif

#endif