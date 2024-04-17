/****************************************************************************
*   Generated by ACUITY #ACUITY_VERSION#
*
*   Neural Network appliction post-process source file
****************************************************************************/
/*-------------------------------------------
                Includes
-------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vnn_global.h"
#include "vnn_post_process.h"

#define _BASETSD_H

/*-------------------------------------------
                  Functions
-------------------------------------------*/
vip_uint32_t shape_to_string(
    vip_uint32_t   *shape,
    vip_uint32_t   dim_num,
    char          *buf,
    vip_uint32_t   buf_sz,
    vip_bool_e     for_print)
{
#define _PRINT_FMT     (0)
#define _NOT_PRINT_FMT (1)
    vip_uint32_t s;
    vip_uint32_t count;
    const char * all_fmt[] = {" %d,", "%d_" };
    const char * fmt;
    if( NULL == shape || NULL == buf
        || dim_num == 0 || buf_sz == 0 )
    {
        return 0;
    }
    if( vip_false_e == for_print )
    {
        fmt = all_fmt[_NOT_PRINT_FMT];
    }
    else
    {
        fmt = all_fmt[_PRINT_FMT];
    }
    count = 0;
    for( s = 0; s < dim_num; s++ )
    {
        if( count >= buf_sz )
        {
            break;
        }
        count += snprintf( &buf[count], buf_sz - count,
            fmt, shape[s] );
    }
    buf[count - 1] = 0;
    return count;
}

unsigned int save_file(
    const char *name,
    void *data,
    unsigned int size)
{
    FILE *fp = fopen(name, "wb+");
    unsigned int saved = 0;

    if (fp != NULL) {
        saved = fwrite(data, size, 1, fp);
        fclose(fp);
    } else {
        printf("Saving file %s failed.\n", name);
    }

    return saved;
}

vip_status_e save_output_data(
    vip_network_items *network_items)
{
    vip_status_e status = VIP_SUCCESS;
    int i = 0;
#define _DUMP_FILE_LENGTH 1028
#define _DUMP_SHAPE_LENGTH 128
    char filename[_DUMP_FILE_LENGTH] = {0}, shape[_DUMP_SHAPE_LENGTH] = {0};
    int buff_size = 0;
    void *out_data = NULL;
    vip_buffer_create_params_t buf_param;

    for (i = 0; i < network_items->output_count; i++)
    {
        buff_size = vip_get_buffer_size(network_items->output_buffers[i]);
        if (buff_size <= 0)
        {
            status = VIP_ERROR_IO;
            return status;
        }
        memset(&buf_param, 0, sizeof(buf_param));
        status = vip_query_output(network_items->network, i,
            VIP_BUFFER_PROP_DATA_FORMAT, &buf_param.data_format);
        _CHECK_STATUS(status, final);
        status = vip_query_output(network_items->network, i,
            VIP_BUFFER_PROP_NUM_OF_DIMENSION, &buf_param.num_of_dims);
        _CHECK_STATUS(status, final);
        status = vip_query_output(network_items->network, i,
            VIP_BUFFER_PROP_SIZES_OF_DIMENSION, buf_param.sizes);
        _CHECK_STATUS(status, final);

        shape_to_string( buf_param.sizes, buf_param.num_of_dims,
            shape, _DUMP_SHAPE_LENGTH, vip_false_e );
        snprintf(filename, _DUMP_FILE_LENGTH, "output%u_%s.dat", i, shape);

        out_data = vip_map_buffer(network_items->output_buffers[i]);
        save_file(filename, out_data, buff_size);
    }

final:
    return status;
}

vip_status_e destroy_network(
    vip_network_items *network_items)
{
    vip_status_e status = VIP_SUCCESS;
    int i = 0;

    status = vip_finish_network(network_items->network);
    _CHECK_STATUS(status, final);
    status = vip_destroy_network(network_items->network);
    _CHECK_STATUS(status, final);

    for (i = 0; i < network_items->input_count; i++)
    {
        status = vip_destroy_buffer(network_items->input_buffers[i]);
        _CHECK_STATUS(status, final);
    }
    if (network_items->input_buffers) {
        free(network_items->input_buffers);
        network_items->input_buffers = VIP_NULL;
    }
    for (i = 0; i < network_items->output_count; i++)
    {
        status = vip_destroy_buffer(network_items->output_buffers[i]);
        _CHECK_STATUS(status, final);
    }
    if (network_items->output_buffers) {
        free(network_items->output_buffers);
        network_items->output_buffers = VIP_NULL;
    }

final:
    return status;
}

void destroy_network_items(
    vip_network_items *network_items)
{
    if (network_items->nbg_name) {
        free(network_items->nbg_name);
        network_items->nbg_name = VIP_NULL;
    }
    if (network_items->input_names) {
        free(network_items->input_names);
        network_items->input_names = VIP_NULL;
    }
    if (network_items) {
        free(network_items);
        network_items = VIP_NULL;
    }
}