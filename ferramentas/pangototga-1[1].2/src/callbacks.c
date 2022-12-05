#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>

#include "callbacks.h"
#include "interface.h"
#include "support.h"

extern GtkWidget *window1;
extern GtkWidget *filechooserdialog1;

void
on_button2_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{
	gtk_widget_show(filechooserdialog1);
}


void
on_button3_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{
	gtk_main_quit ();
}


gboolean
on_filechooserdialog1_delete_event     (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{

	gtk_widget_hide(filechooserdialog1);
	return TRUE;
}

void
get_real_pixel_size(GtkWidget *widget, PangoLayout *layout, int *l, int *t, int *r, int *b)
{
	int w, h;
	pango_layout_get_pixel_size(layout, &w, &h);
	if (!w)
	    w += 1;
	if (!h)
	    h += 1;
	w *= 3;
	h *= 3;
	GdkPixmap *pixmap = gdk_pixmap_new(widget->window, w, h, -1);

	GdkGC *gc = gdk_gc_new(pixmap);
	GdkColor color_transparent = {0, 65535, 65535, 65535};
	GdkColor color_font = {0, 0, 0, 0};

	gdk_gc_set_rgb_fg_color(gc, &color_transparent);
	gdk_draw_rectangle(pixmap, gc, TRUE, 0, 0, w, h);

	gdk_gc_set_rgb_fg_color(gc, &color_font);
	gdk_draw_layout(pixmap, gc, w / 3, h / 3, layout);

	GdkPixbuf *pixbuf = gdk_pixbuf_get_from_drawable(NULL,
		pixmap, NULL, 0, 0, 0, 0, w, h);
	guchar *pixels = gdk_pixbuf_get_pixels(pixbuf);

	int rowstride = gdk_pixbuf_get_rowstride (pixbuf);

	*l = w;
	*t = h;
	*r = -1;
	*b = -1;
	int x, y;
	for (y = 0; y < h; y++)
	{
		for (x = 0; x < w; x++)
		{
			guchar red = pixels[rowstride * y + x * 3];
			guchar green = pixels[rowstride * y + x * 3 + 1];
			guchar blue = pixels[rowstride * y + x * 3 + 2];
			if (red == 255 && green == 255 && blue == 255)
				continue;
			if (x < *l)
				*l = x;
			if (y < *t)
				*t = y;
			if (x > *r)
				*r = x;
			if (y > *b)
				*b = y;
		}
	}

	g_object_unref(pixbuf);
	g_object_unref(gc);
	g_object_unref(pixmap);

	if (*r >= 0)
	{
		*l -= w / 3;
		*t -= h / 3;
		*r -= w / 3;
		*b -= h / 3;
	}
	else
	{
		*l = *t = *r = *b = 0;
	}
}


void
on_button5_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkWidget *widget = window1;
	gchar const *filename =
	gtk_file_chooser_get_filename (GTK_FILE_CHOOSER(filechooserdialog1));
	gtk_widget_hide (filechooserdialog1);

	GtkWidget *imagewidget, *fontsel;
	gchar *fontname;
	
	imagewidget = lookup_widget (widget, "image1");
	fontsel = lookup_widget (widget, "fontselection1");
	int first_char = strtol(gtk_entry_get_text(GTK_ENTRY(lookup_widget (widget, "entry1"))), NULL, 0);
	int last_char = strtol(gtk_entry_get_text(GTK_ENTRY(lookup_widget (widget, "entry2"))), NULL, 0);
	int first_col = strtol(gtk_entry_get_text(GTK_ENTRY(lookup_widget (widget, "entry3"))), NULL, 0);
	int last_col = strtol(gtk_entry_get_text(GTK_ENTRY(lookup_widget (widget, "entry4"))), NULL, 0);
        int truecolor = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(lookup_widget (widget, "checkbutton1")));

	fontname = gtk_font_selection_get_font_name (GTK_FONT_SELECTION (fontsel));

	printf("Exporting font %s.\n", fontname);

	PangoLayout *layout = gtk_widget_create_pango_layout(imagewidget, "?");

	PangoFontDescription *desc = pango_font_description_from_string(fontname);

	pango_layout_set_font_description(layout, desc);

	int shift_down = 0;
	int gridw = 1, gridh = 1;
	int i;
	for (i = first_char; i <= last_char; i++)
	{
		gchar utf8[10];
		pango_layout_set_text(layout, utf8, g_unichar_to_utf8(i, utf8));

		int w, h;
		pango_layout_get_pixel_size(layout, &w, &h);
		/* can't trust the above function, unfortunately */

		int l, t, r, b, rw, rh;
                get_real_pixel_size(imagewidget, layout, &l, &t, &r, &b);
		rw = 1 + r;
		rh = 1 + b;
		if (l < 0)
			rw -= l;
		if (t < 0)
			rh -= t;

		if (rw > w)
			w = rw;
		if (rh > h)
			h = rh;

		if (w > gridw)
			gridw = w;
		if (h > gridh)
			gridh = h;

		if (t < shift_down)
			shift_down = t;
	}

	int tga_w = (gridw + 1) * 16 + 1;
	int tga_h = (1 + (last_char - first_char) / 16) * (gridh + 1) + 1;
	int tga_x = 1;
	int tga_y = 1;

	if (shift_down >= 0)
		shift_down = 0;

	if (shift_down)
		printf("Shifting all characters down by %d.\n", shift_down);

	printf("TGA: %d x %d\n", tga_w, tga_h);

	GdkPixmap *pixmap = gdk_pixmap_new(imagewidget->window, tga_w, tga_h, -1);

	GdkGC *gc = gdk_gc_new(pixmap);
	GdkColor color_back = {0, 65535, 0, 65535};
	GdkColor color_transparent = {0, 65535, 65535, 65535};
	GdkColor color_font = {0, 0, 0, 0};

	gdk_gc_set_rgb_fg_color(gc, &color_back);
	gdk_draw_rectangle(pixmap, gc, TRUE, 0, 0, tga_w, tga_h);

	for (i = first_char; i <= last_char; i++)
	{
		gchar utf8[10] = {0};
		pango_layout_set_text(layout, utf8, g_unichar_to_utf8(i, utf8));

		int w, h;
		pango_layout_get_pixel_size(layout, &w, &h);

		int l, t, r, b;
                get_real_pixel_size(imagewidget, layout, &l, &t, &r, &b);

		if (l >= 0)
			l = 0;

		r = 1 + r;
		if (r <= w)
			r = w;

		if (t >= 0)
			t = 0;

		b = 1 + b;
		if (b <= h)
			b = h;

		if (l || t || r != w || b != h)
		{
			printf("Char %d (%s): Oversize by %d %d %d %d pixel!\n", i, utf8, -l, -t, r - w, b - h);
		}
		w = r - l;
		h = gridh;

		GdkRectangle rect = {tga_x, tga_y, w, h};
		gdk_gc_set_clip_rectangle(gc, &rect);

		gdk_gc_set_rgb_fg_color(gc, &color_transparent);
		gdk_draw_rectangle(pixmap, gc, TRUE, tga_x, tga_y, w, h);

		gdk_gc_set_rgb_fg_color(gc, &color_font);

		gdk_draw_layout(pixmap, gc, tga_x - l, tga_y - shift_down + t, layout);

		tga_x += gridw + 1;
		if (tga_x >= tga_w)
		{
			tga_x = 1;
			tga_y += gridh + 1;
		}
	}

	gtk_widget_set_size_request(imagewidget, tga_w, tga_h);
	gtk_image_set_from_pixmap(GTK_IMAGE(imagewidget), pixmap, NULL);

	GdkPixbuf *pixbuf = gdk_pixbuf_get_from_drawable(NULL,
		pixmap, NULL, 0, 0, 0, 0, tga_w, tga_h);
	guchar *pixels = gdk_pixbuf_get_pixels(pixbuf);

	int rowstride = gdk_pixbuf_get_rowstride (pixbuf);

	FILE *file = fopen(filename, "wb");
	guchar tga_header[] = {0, truecolor ? 0 : 1,
		truecolor ? 2 : 1, 0, 0, 0, truecolor ? 0 : 1,
		truecolor ? 0 : 24,
		0, 0, 0, 0, tga_w & 255, tga_w >> 8, tga_h & 255, tga_h >> 8,
		truecolor ? 32 : 8, truecolor ?  8 : 0};
	fwrite(tga_header, sizeof(tga_header), 1, file);

	if (!truecolor)
	{
		int c;
		for (c = 0; c < 256; c++)
		{
			int r = 0, g = 0, b = 0;
			if (c == 0) {r = 255; g = 255; b = 0;}
			else if (c == 255) {r = 255; g = 0; b = 255;}
			else if (c >= first_col && c <= last_col)
			{
				r = g = b = 255 - (c - first_col) * 255 / (last_col - first_col);
			}
			fputc(b, file);
			fputc(g, file);
			fputc(r, file);
		}
	}
	int x, y;
	for (y = tga_h - 1; y >= 0; y--)
	{
		for (x = 0; x < tga_w; x++)
		{
			guchar red = pixels[rowstride * y + x * 3];
			guchar green = pixels[rowstride * y + x * 3 + 1];
			guchar blue = pixels[rowstride * y + x * 3 + 2];
			if (red == 255 && green == 0 && blue == 255)
			{
				if (truecolor)
				{
					fputc(255, file);
					fputc(255, file);
					fputc(255, file);
					fputc(255, file);
				}
				else
					fputc(255, file);
			}
			else if (red == 255 && green == 255 && blue == 255)
			{
				if (truecolor)
				{
					fputc(0, file);
					fputc(0, file);
					fputc(0, file);
					fputc(0, file);
				}
				else
					fputc(0, file);
			}
			else
			{
				int c = (red + green + blue) / 3;
				if (truecolor)
				{
					fputc(0, file);
					fputc(0, file);
					fputc(0, file);
					fputc(255 - c, file);
				}
				else
					fputc(last_col - c * (last_col - first_col) / 255, file);
			}
		}
	}
	fclose(file);

	g_object_unref(pixbuf);
	g_object_unref(layout);
	g_object_unref(gc);
	g_object_unref(pixmap);
}


void
on_button4_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{
	gtk_widget_hide(filechooserdialog1);
}

