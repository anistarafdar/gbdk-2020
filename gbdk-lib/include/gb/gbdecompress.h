/** @file gb/gbcompress.h
    
    GB-Compress decompressor
    Compatible with the compression used in GBTD
*/

#ifndef __GBDECOMPRESS_H_INCLUDE
#define __GBDECOMPRESS_H_INCLUDE

/** gb-decompress data from sour into dest

    @param sour   Pointer to source gb-compressed data
    @param dest   Pointer to destination buffer/address

    @see gb_decompress_bkg_data, gb_decompress_win_data, gb_decompress_sprite_data
 */
void gb_decompress(const unsigned char * sour, unsigned char * dest) __preserves_regs(b, c);


/** gb-decompress background tiles into VRAM

    @param first_tile  Index of the first tile to write
    @param sour        Pointer to (gb-compressed 2 bpp) source Tile Pattern data.

    Note: This function avoids writes during modes 2 & 3

    @see gb_decompress_bkg, gb_decompress_win_data, gb_decompress_sprite_data
*/
void gb_decompress_bkg_data(UINT8 first_tile, const unsigned char * sour) __preserves_regs(b, c);


/** gb-decompress window tiles into VRAM

    @param first_tile  Index of the first tile to write
    @param sour        Pointer to (gb-compressed 2 bpp) source Tile Pattern data.

    This is the same as @ref gb_decompress_bkg_data, since the Window Layer and
    Background Layer share the same Tile pattern data.

    Note: This function avoids writes during modes 2 & 3

    @see gb_decompress, gb_decompress_bkg_data, gb_decompress_sprite_data
 */
void gb_decompress_win_data(UINT8 first_tile, const unsigned char * sour) __preserves_regs(b, c);


/** gb-decompress sprite tiles into VRAM

    @param first_tile  Index of the first tile to write
    @param sour        Pointer to source compressed data

    Note: This function avoids writes during modes 2 & 3

    @see gb_decompress, gb_decompress_bkg_data, gb_decompress_win_data
 */
void gb_decompress_sprite_data(UINT8 first_tile, const unsigned char * sour) __preserves_regs(b, c);

#endif