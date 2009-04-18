#ifndef IHEXPARSER_H_
#define IHEXPARSER_H_

/************************************************************************
 title:		iHex Parser
 file:     	iHexParser.h
 sw: 		AVR32-GCC
 hw: 		NGW100
 license:  	copyright(c) 2009 Team eHalOS
 usage:    	see doxygen manual

 LICENSE:
 This program is a free software;
 ************************************************************************/

/**
 *  @defgroup KERNEL Halos Kernel
 */

/**
 *  @defgroup VMM Virtual Memory Management
 *  @ingroup KERNEL
 */

/**
 *  @defgroup VMM_HEX iHex Parser
 *  @ingroup VMM
 *  @brief iHex Parser. Should transform iHex Process Images to Bin Process Images.
 *  Not used at the moment.
 *
 *  @note ...
 *  @author drb8w
 *  @version 0.0.1
 */

/**@{*/

#ifdef __cplusplus
extern "C" {
#endif

	#include<stdio.h>

	int myExp(int basis, int exponent);

	unsigned char ascii_to_dec(char hex);

	int hex_to_dec(char* buffer, int size);

	int read_start_code(char** hexbuffer);

	int read_byte_count(char** hexbuffer);

	int read_address(char** hexbuffer);

	int read_type(char** hexbuffer);

	void read_data(char** hexbuffer, char* data, int count);

	void clear_buffer(char* buffer, int count);

	int check_checksum(char** hexbuffer, char* checksumstart, int bytecount);

	void copy_data_to_ram(char* data, int baseaddress,  int count);

	void write_to_disk(int len);

#ifdef __cplusplus
} /* closing brace for extern "C" */
#endif

#endif /*IHEXPARSER_H_*/
