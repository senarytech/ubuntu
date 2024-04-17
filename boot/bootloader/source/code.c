/*
 * NDA AND NEED-TO-KNOW REQUIRED
 *
 * Copyright © 2013-2018 Synaptics Incorporated. All rights reserved.
 *
 * This file contains information that is proprietary to Synaptics
 * Incorporated ("Synaptics"). The holder of this file shall treat all
 * information contained herein as confidential, shall use the
 * information only for its intended purpose, and shall not duplicate,
 * disclose, or disseminate any of this information in any manner
 * unless Synaptics has otherwise provided express, written
 * permission.
 *
 * Use of the materials may require a license of intellectual property
 * from a third party or from Synaptics. This file conveys no express
 * or implied licenses to any intellectual property rights belonging
 * to Synaptics.
 *
 * INFORMATION CONTAINED IN THIS DOCUMENT IS PROVIDED "AS-IS," AND
 * SYNAPTICS EXPRESSLY DISCLAIMS ALL EXPRESS AND IMPLIED WARRANTIES,
 * INCLUDING ANY IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE, AND ANY WARRANTIES OF NON-INFRINGEMENT OF ANY
 * INTELLECTUAL PROPERTY RIGHTS. IN NO EVENT SHALL SYNAPTICS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, PUNITIVE, OR
 * CONSEQUENTIAL DAMAGES ARISING OUT OF OR IN CONNECTION WITH THE USE
 * OF THE INFORMATION CONTAINED IN THIS DOCUMENT, HOWEVER CAUSED AND
 * BASED ON ANY THEORY OF LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * NEGLIGENCE OR OTHER TORTIOUS ACTION, AND EVEN IF SYNAPTICS WAS
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. IF A TRIBUNAL OF
 * COMPETENT JURISDICTION DOES NOT PERMIT THE DISCLAIMER OF DIRECT
 * DAMAGES OR ANY OTHER DAMAGES, SYNAPTICS' TOTAL CUMULATIVE LIABILITY
 * TO ANY PARTY SHALL NOT EXCEED ONE HUNDRED U.S. DOLLARS.
 */
#include <stdio.h>
#include <stdlib.h>

int main( void )
{
	FILE *fr, *fw;
	int data = 0, size = 0 , i = 0;
	int ret = 0;

	fr = fopen( "figo_sram.bin", "r" );
	if (NULL == fr){
		printf("can't open files figo_sram.bin!\n");
		ret = -1;
		goto fr_error;
	}

	fw = fopen( "figo_sram_bin.c", "w" );
	if (NULL == fw){
		printf("can't open files figo_sram.c!\n");
		ret = -1;
		goto fw_error;
	}
	//fw = fopen( "build/SM_Code.c", "w" );

	fprintf( fw, "#include \"com_type.h\"\n\nunsigned int FIGO_Sram_Code[]= {\n" );

	for( i = 0 ; ; i ++ )
	{
		size = fread( &data, 1, 4, fr );
		if( size == 0 ) break;

		fprintf( fw, "0x%8.8x, ", data );
		if( i % 8 == 7 )
			fprintf( fw, "\n" );
	}

	fprintf( fw, "};\n\nunsigned int Figo_Sram_Code_Size = %d;\n", i );

	fclose( fw );
fw_error:
	fclose( fr );
fr_error:
	return ret;
}
