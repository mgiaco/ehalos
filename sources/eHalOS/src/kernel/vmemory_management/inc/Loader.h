#ifndef LOADER_H_
#define LOADER_H_

/************************************************************************
 title:		Loader
 file:     	Loader.h
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
 *  @defgroup VMM_LDR	Loader
 *  @ingroup VMM
 *  @brief Loader. The Loader loads the complete Process Image into Virtual Memory.
 *  This causes multiple initial page faults which actualize the TLB.
 *
 *  @note ...
 *  @author drb8w
 *  @version 0.0.1
 */

/**@{*/


#ifdef __cplusplus
extern "C" {
#endif

#include "Utils.h"
#include "PageTableEntry.h"
#include "InvertedPageTable.h"
#include "PageTable.h"


/**
 * @struct Binary Image description for process image to load it from flash to memory, assumes that bin image is continuous.
 */
typedef struct _BinImage
{
	long loadAddress; // load address on flash
	long baseAddress; // base address in RAM
	long size;		  // size in bytes
	long dataOffset;  // virtual address of first data, before text
	char *pData;      // pointer to start of image data
	char *pImageName;  // name of app image
}BinImage;

typedef BinImage *BinImagePtr;
typedef BinImagePtr *BinImageHdl;

typedef void *IHexImagePtr;

/**
 * Load raw process image into memory.
 * Stack and heap allocation is done by application.
 * Steps:
 * + reserve pages for BinImage
 * + write BinImage to Virtual Ram
 */
BOOL loadBinApplication(BinImagePtr pBinImage, int ASID);

/**
 * BinImage is nec if e.g. startaddress is not 0x0000.
 * Unused at the moment.
 */
void startBinApplication(BinImagePtr pBinImage, int ASID);

/**
 * Unused at the moment.
 */
void stopBinApplication(BinImagePtr pBinImage, int ASID);

/**
 * Unloads the binary process image from memory. That means remove it from the page table system.
 * Unused at the moment.
 */
BOOL unloadBinApplication(BinImagePtr pBinImage, int ASID);

/**
 * Determines the necessary number of pages of the BinImage and reserves them in the Inverted Page Table.
 * @param pBinImage the binary process image
 * @param ASID the ASID for the corresponding running process
 */
BOOL reservePagesForBinImage(BinImagePtr pBinImage, int ASID);

/**
 * Writes raw binary image to ram.
 * During writing multiple page faults will occur.
 * Because context is the context of OS not of application
 * it is necessary to use the actual ASID also in OS Mode.
 * The image is written from back to front to assure that the start page
 * is in the TLB at prosess start.
 * @param pBinImage the image that is written to virtual Ram
 * @param ASID the PID of the corresponding process
 */
BOOL writeBinImageToVirtualRam(BinImagePtr pBinImage, int ASID);

/**
 * Determines how many pages must be reserved for that binary process image.
 * @param pBinImage the image to be analysed
 */
int nrOfPagesForBinImage(BinImagePtr pBinImage);


#ifdef __cplusplus
} /* closing brace for extern "C" */
#endif

#endif /*LOADER_H_*/
