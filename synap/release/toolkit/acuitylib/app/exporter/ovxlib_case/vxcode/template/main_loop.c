/****************************************************************************
*   Generated by ACUITY #ACUITY_VERSION#
*   Match ovxlib #OVXLIB_VERSION#
*
*   Neural Network application project entry file
****************************************************************************/
/*-------------------------------------------
                Includes
-------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef __linux__
#include <time.h>
#include <inttypes.h>
#elif defined(_WIN32)
#include <windows.h>
#endif

#define _BASETSD_H

#include "vsi_nn_pub.h"

#include "vnn_global.h"
#include "vnn_pre_process.h"
#include "vnn_post_process.h"
#include "vnn_#NETWORK_NAME_LOWER#.h"

/*-------------------------------------------
        Macros and Variables
-------------------------------------------*/
#ifdef __linux__
#define VSI_UINT64_SPECIFIER PRIu64
#elif defined(_WIN32)
#define VSI_UINT64_SPECIFIER "I64u"
#endif

#define MEASURE_TIME_SUPPORT 1

#if( MEASURE_TIME_SUPPORT )
    #define __STRCAT(_X, _Y) _X##_Y
    #define _STRCAT(_X, _Y) __STRCAT(_X, _Y)
    #define MEASURE_ELAPSED_TIME( __COMMENT, __CODE__ ) do {                                         \
            uint64_t _STRCAT(tmsStart_, __LINE__), _STRCAT(tmsEnd_, __LINE__);                     \
            float _STRCAT(msVal_, __LINE__), _STRCAT(usVal_, __LINE__);                        \
            _STRCAT(tmsStart_, __LINE__) = get_perf_count();                                        \
            do { __CODE__ } while ( 0 );                                                            \
            _STRCAT(tmsEnd_, __LINE__) = get_perf_count();                                          \
            _STRCAT(msVal_, __LINE__) = (_STRCAT(tmsEnd_, __LINE__) - _STRCAT(tmsStart_, __LINE__)) / 1000000.0f;   \
            _STRCAT(usVal_, __LINE__) = (_STRCAT(tmsEnd_, __LINE__) - _STRCAT(tmsStart_, __LINE__)) / 1000.0f;      \
            printf("%s: %.4fms or %.4fus\n", __COMMENT, _STRCAT(msVal_, __LINE__), _STRCAT(usVal_, __LINE__));       \
        } while( 0 )
#else
    #define MEASURE_ELAPSED_TIME( __COMMENT, __CODE__ ) do { \
            __CODE__                                        \
        } while( 0 )
#endif

#ifndef SAFE_FREE
    #define SAFE_FREE(_PTR) {if((_PTR) != NULL){free(_PTR); (_PTR) = NULL;}}
#endif

#ifndef VSI_SAFE_FREE
    #define VSI_SAFE_FREE(_PTR) {if((_PTR) != NULL){vsi_nn_Free(_PTR); (_PTR) = NULL;}}
#endif

/*-------------------------------------------
                  Functions
-------------------------------------------*/
static void vnn_ReleaseNeuralNetwork
    (
    vsi_nn_graph_t *graph
    )
{
    vnn_Release#NETWORK_NAME#( graph, TRUE );
    if (vnn_UseImagePreprocessNode())
    {
        vnn_ReleaseBufferImage();
    }
}

static vsi_status vnn_PostProcessNeuralNetwork
    (
    vsi_nn_graph_t *graph
    )
{
    return vnn_PostProcess#NETWORK_NAME#( graph );
}

#define BILLION                                 1000000000
static uint64_t get_perf_count()
{
#if defined(__linux__) || defined(__ANDROID__) || defined(__QNX__) || defined(__CYGWIN__)
    struct timespec ts;

    clock_gettime(CLOCK_MONOTONIC, &ts);

    return (uint64_t)((uint64_t)ts.tv_nsec + (uint64_t)ts.tv_sec * BILLION);
#elif defined(_WIN32) || defined(UNDER_CE)
    LARGE_INTEGER ln;

    QueryPerformanceCounter(&ln);

    return (uint64_t)ln.QuadPart;
#endif
}

static vsi_status vnn_ProcessGraph
    (
    vsi_nn_graph_t *graph
    )
{
    vsi_status status;
    int32_t i,loop;
    char *loop_s;
    uint64_t tmsStart, tmsEnd, sigStart, sigEnd;
    float msVal, usVal;

    status = VSI_FAILURE;
    loop = 1; /* default loop time is 1 */
    loop_s = getenv("VNN_LOOP_TIME");
    if(loop_s)
    {
        loop = atoi(loop_s);
    }

    /* Verify graph */
    printf("Verify...\n");
    MEASURE_ELAPSED_TIME("Verify Graph",
    status = vsi_nn_VerifyGraph( graph );
    );
    TEST_CHECK_STATUS( status, final );

    /* Run graph */
    tmsStart = get_perf_count();
    printf("Start run graph [%d] times...\n", loop);
    for(i = 0; i < loop; i++)
    {
        sigStart = get_perf_count();
        status = vsi_nn_RunGraph( graph );
        if(status != VSI_SUCCESS)
        {
            printf("Run graph the %d time fail\n", i);
        }
        TEST_CHECK_STATUS( status, final );

        sigEnd = get_perf_count();
        msVal = (sigEnd - sigStart)/(float)1000000;
        usVal = (sigEnd - sigStart)/(float)1000;
        printf("Run the %u time: %.4fms or %.4fus\n", (i + 1), msVal, usVal);
    }
    tmsEnd = get_perf_count();
    msVal = (tmsEnd - tmsStart)/(float)1000000;
    usVal = (tmsEnd - tmsStart)/(float)1000;
    printf("vxProcessGraph execution time:\n");
    printf("Total   %.4fms or %.4fus\n", msVal, usVal);
    printf("Average %.4fms or %.4fus\n", ((float)usVal)/1000/loop, ((float)usVal)/loop);

final:
    return status;
}

static vsi_status vnn_PreProcessNeuralNetwork
    (
    vsi_nn_graph_t *graph,
    int argc,
    char **argv
    )
{
    /*
     * argv0:   execute file
     * argv1:   data file
     * argv2~n: inputs n file
     */
    const char **inputs = (const char **)argv + 2;
    uint32_t input_num = argc - 2;
    /*
    if(vnn_UseImagePreprocessNode())
    {
        return vnn_PreProcess#NETWORK_NAME#_ImageProcess(graph, inputs, input_num);
    }
    */
    return vnn_PreProcess#NETWORK_NAME#( graph, inputs, input_num );
}

static vsi_nn_graph_t *vnn_CreateNeuralNetwork
    (
    const char *data_file_name
    )
{
    vsi_nn_graph_t *graph = NULL;

    MEASURE_ELAPSED_TIME("Create Neural Network:",
    graph = vnn_Create#NETWORK_NAME#( data_file_name, NULL,
                      vnn_GetPreProcessMap(), vnn_GetPreProcessMapCount(),
                      vnn_GetPostProcessMap(), vnn_GetPostProcessMapCount() ););
    TEST_CHECK_PTR( graph, final );

    /* Show the node and tensor */
    vsi_nn_PrintGraph( graph );

final:
    return graph;
}

static vsi_status vnn_VerifyGraph
    (
    vsi_nn_graph_t* graph
    )
{
    vsi_status status;

    /* Verify graph */
    printf("Verify...\n");
    MEASURE_ELAPSED_TIME("Verify Graph",
    status = vsi_nn_VerifyGraph( graph );
    );
    TEST_CHECK_STATUS( status, final );

final:
    return status;
}

/*-------------------------------------------
             User data & Functions
-------------------------------------------*/
typedef struct
{
    uint8_t * input_buffer;
    vsi_size_t input_buffer_sz;
    vsi_nn_dtype_t input_dtype;
} vnn_user_data_t;

/*-------------------------------------------
Procedure to prepare input data, return FALSE
to end loop
-------------------------------------------*/
vsi_bool prepare_input_data
    (
    vsi_nn_graph_t* graph,
    uint32_t iteration,
    void* user_data
    )
{
    vnn_user_data_t* u = (vnn_user_data_t *)user_data;
    vsi_nn_tensor_t* tensor = NULL;
    vsi_size_t input_element_num = 0;
    vsi_size_t input_sz_per_iteration = 0;
    uint8_t* cur_input_buf = NULL;

    tensor = vsi_nn_GetTensor(graph, graph->input.tensors[0]);
    input_element_num       = vsi_nn_GetElementNum(tensor);
    input_sz_per_iteration  = input_element_num * vsi_nn_GetTypeBytes(u->input_dtype.vx_type);
    cur_input_buf           = u->input_buffer + iteration * input_sz_per_iteration;

    if(cur_input_buf < u->input_buffer + u->input_buffer_sz)
    {
        return VSI_SUCCESS == vsi_nn_CopyRawDataToTensor( graph, cur_input_buf, &u->input_dtype, tensor );
    }
    return FALSE;
}

/*-------------------------------------------
Procedure to process output data, return FALSE
to end loop
-------------------------------------------*/
vsi_bool process_output_data
    (
    vsi_nn_graph_t* graph,
    uint32_t iteration,
    void* user_data
    )
{
    char* output_fname = "output.bin";

    FILE* fp = NULL;
    vsi_size_t tensor_element_size = 0;
    float* data = NULL;
    uint8_t output_tensor_index = 0; //FIXME: Please set the graph output id
    vsi_nn_tensor_t *tensor = NULL;

    fp = fopen(output_fname, iteration == 0 ? "wb" : "ab+");
    if(!fp)
    {
        VSILOGE("open file %s fail!\n", output_fname);
        return FALSE;
    }

    tensor = vsi_nn_GetTensor( graph, graph->output.tensors[output_tensor_index] );
    tensor_element_size = vsi_nn_GetElementNum(tensor);
    data = vsi_nn_ConvertTensorToFloat32Data( graph, tensor );

    fseek(fp, 0, SEEK_END);
    fwrite(data, 1, tensor_element_size * sizeof(float), fp);
    fclose(fp);

    VSI_SAFE_FREE(data);

    return TRUE;
}

/*-------------------------------------------
                  Main Functions
-------------------------------------------*/
int main
    (
    int argc,
    char **argv
    )
{
    vsi_status status = VSI_FAILURE;
    vsi_nn_graph_t *graph;
    const char *data_name = NULL;
    const char *image_name = NULL;
    const char *pos = NULL;
    vsi_size_t input_sz_per_iteration = 0;
    uint8_t* input_buf = NULL;
    vsi_size_t input_buf_sz = 0;
    vsi_size_t input_element_num = 0;
    vnn_user_data_t user_data;
    vsi_nn_tensor_t* tensor = NULL;
    uint32_t iteration = 0;
    uint32_t iteration_count = 0;
    vsi_bool exit_loop = 0;

    if(argc < 3)
    {
        printf("Usage: %s data_file inputs...\n", argv[0]);
        return -1;
    }

    data_name = (const char *)argv[1];
    image_name = (const char *)argv[2];

    /* Create the neural network */
    graph = vnn_CreateNeuralNetwork( data_name );
    TEST_CHECK_PTR( graph, final );

    /* Verify graph */
    status = vnn_VerifyGraph( graph );
    TEST_CHECK_STATUS( status, final );
#EXTERNAL_CONNECTIONS#
    /* Initialize user data */
    memset(&user_data, 0x00, sizeof(user_data));
    user_data.input_dtype.qnt_type = VSI_NN_QNT_TYPE_NONE;
    user_data.input_dtype.vx_type = VSI_NN_TYPE_FLOAT16;

    /* Initialize input data */
    if( ((pos = strstr(image_name, ".tensor")) && (0 == strcmp(pos, ".tensor")))
        || ((pos = strstr(image_name, ".txt")) && (0 == strcmp(pos, ".txt"))))
    {
        /* load data from .tensor file */
        input_buf_sz = vnn_LoadFP32DataFromTextFile( image_name, &input_buf, &input_buf_sz );
        user_data.input_dtype.vx_type = VSI_NN_TYPE_FLOAT32;
    }
    else
    {
        input_buf_sz = vnn_LoadRawDataFromBinaryFile( image_name, &input_buf, &input_buf_sz );
    }

    tensor = vsi_nn_GetTensor(graph, graph->input.tensors[0]);
    input_element_num = vsi_nn_GetElementNum( tensor );
    input_sz_per_iteration = input_element_num * vsi_nn_GetTypeBytes( user_data.input_dtype.vx_type );

    if( input_buf_sz % input_sz_per_iteration != 0 )
    {
        VSILOGE("Input data size mismatch %u:%u\n", input_buf_sz, input_sz_per_iteration);
        status = VSI_FAILURE;
        TEST_CHECK_STATUS(status, final);
    }

    user_data.input_buffer = input_buf;
    user_data.input_buffer_sz = input_buf_sz;

    /* Process graph */
    iteration_count = (uint32_t)(input_buf_sz / input_sz_per_iteration);

    /* Get iteration count from env variable */
    pos = getenv("ITERATION");
    if(pos)
    {
        iteration_count = atoi(pos);
    }

    /* Force iteration count to 1 if saving nbg */
    pos = getenv("VIV_VX_ENABLE_SAVE_NETWORK_BINARY");
    if(pos)
    {
        iteration = atoi(pos);
        if(iteration > 0)
        {
            iteration_count = 1;
        }
    }

    printf("Total iteration: %d\n", iteration_count);

    for( iteration = 0; iteration < iteration_count && !exit_loop; iteration++ )
    {
        printf("Iteration %d ", iteration);

        /* feed input data */
        exit_loop = !prepare_input_data(graph, iteration, &user_data);

        /* process graph */
        MEASURE_ELAPSED_TIME("Process graph",
        exit_loop = VSI_SUCCESS != vsi_nn_RunGraph( graph );
        );

        /* fetch output data */
        exit_loop = !process_output_data( graph, iteration, NULL );

        if( VNN_APP_DEBUG )
        {
            /* Dump all node outputs */
            vsi_nn_DumpGraphNodeOutputs( graph, "./network_dump", NULL, 0, TRUE, 0 );
        }
    }

final:
    vnn_ReleaseNeuralNetwork( graph );
    SAFE_FREE(input_buf);
    fflush(stdout);
    fflush(stderr);
    return status;
}
