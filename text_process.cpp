// code from https://github.com/harfbuzz/harfbuzz-tutorial/blob/master/hello-harfbuzz-freetype.c (slightly modified)

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <hb.h>
#include <hb-ft.h>
#include "data_path.hpp"

#define FONT_SIZE 36
#define MARGIN (FONT_SIZE * .5)

int main(int argc, char **argv)
{
  std::string fontfile = data_path("OpenSans-Regular.ttf");
  std::string text = "IF YOU ARE READING THIS, I WOULD LIKE TO APOLOGIZE BECAUSE I DID NOT FINISH THIS GAME BECAUSE DOING WORK IS TOO HARD AND I AM TIRED";
  /* Initialize FreeTy pe and create FreeType font face. */
  FT_Library ft_library;
  FT_Face ft_face;
  FT_Error ft_error;

  ft_error = FT_Init_FreeType (&ft_library);
  if (ft_error) abort();
  ft_error = FT_New_Face (ft_library, fontfile.c_str(), 0, &ft_face);
  if (ft_error) abort();
  ft_error = FT_Set_Char_Size (ft_face, FONT_SIZE*64, FONT_SIZE*64, 0, 0);
  if (ft_error) abort();

  /* Create hb-ft font. */
  hb_font_t *hb_font;
  hb_font = hb_ft_font_create (ft_face, NULL);

  /* Create hb-buffer and populate. */
  hb_buffer_t *hb_buffer;
  hb_buffer = hb_buffer_create ();
  hb_buffer_add_utf8 (hb_buffer, text.c_str(), -1, 0, -1);
  hb_buffer_guess_segment_properties (hb_buffer);

  /* Shape it! */
  hb_shape (hb_font, hb_buffer, NULL, 0);

  /* Get glyph information and positions out of the buffer. */
  unsigned int len = hb_buffer_get_length (hb_buffer);
  hb_glyph_info_t *info = hb_buffer_get_glyph_infos (hb_buffer, NULL);
  hb_glyph_position_t *pos = hb_buffer_get_glyph_positions (hb_buffer, NULL);

  /* Print them out as is. */
  printf ("representation of the following string: %s\n", text.c_str());
  printf ("Raw buffer contents:\n");
  for (unsigned int i = 0; i < len; i++)
  {

    hb_codepoint_t gid   = info[i].codepoint;
    unsigned int cluster = info[i].cluster;
    double x_advance = pos[i].x_advance / 64.;
    double y_advance = pos[i].y_advance / 64.;
    double x_offset  = pos[i].x_offset / 64.;
    double y_offset  = pos[i].y_offset / 64.;

    char glyphname[32];
    hb_font_get_glyph_name (hb_font, gid, glyphname, sizeof (glyphname));

    printf ("glyph='%s'	cluster=%d	advance=(%g,%g)	offset=(%g,%g)\n",
            glyphname, cluster, x_advance, y_advance, x_offset, y_offset);
  }
  return 0;
}