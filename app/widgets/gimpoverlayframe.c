/* GIMP - The GNU Image Manipulation Program
 * Copyright (C) 1995 Spencer Kimball and Peter Mattis
 *
 * gimpoverlayframe.c
 * Copyright (C) 2010  Michael Natterer <mitch@gimp.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "config.h"

#include <gtk/gtk.h>

#include "widgets-types.h"

#include "gimpoverlayframe.h"


static void       gimp_overlay_frame_size_request  (GtkWidget      *widget,
                                                    GtkRequisition *requisition);
static void       gimp_overlay_frame_size_allocate (GtkWidget      *widget,
                                                    GtkAllocation  *allocation);
static gboolean   gimp_overlay_frame_expose        (GtkWidget      *widget,
                                                    GdkEventExpose *eevent);


G_DEFINE_TYPE (GimpOverlayFrame, gimp_overlay_frame, GTK_TYPE_BIN)

#define parent_class gimp_overlay_frame_parent_class


static void
gimp_overlay_frame_class_init (GimpOverlayFrameClass *klass)
{
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  widget_class->size_request  = gimp_overlay_frame_size_request;
  widget_class->size_allocate = gimp_overlay_frame_size_allocate;
  widget_class->expose_event  = gimp_overlay_frame_expose;
}

static void
gimp_overlay_frame_init (GimpOverlayFrame *frame)
{
  GtkWidget   *widget = GTK_WIDGET (frame);

#if 0 /* crashes badly beause gtk+ doesn't support offscreen windows
       * with colormap != parent_colormap yet
       */
  GdkScreen   *screen = gtk_widget_get_screen (widget);
  GdkColormap *rgba   = gdk_screen_get_rgba_colormap (screen);

  if (rgba)
    gtk_widget_set_colormap (widget, rgba);
#endif

  gtk_widget_set_app_paintable (widget, TRUE);
}

static void
gimp_overlay_frame_size_request (GtkWidget      *widget,
                                 GtkRequisition *requisition)
{
  GtkContainer   *container = GTK_CONTAINER (widget);
  GtkWidget      *child     = gtk_bin_get_child (GTK_BIN (widget));
  GtkRequisition  child_requisition;
  gint            border_width;

  border_width = gtk_container_get_border_width (container);

  requisition->width  = border_width * 2;
  requisition->height = border_width * 2;

  if (child && gtk_widget_get_visible (child))
    {
      gtk_widget_size_request (child, &child_requisition);
    }
  else
    {
      child_requisition.width  = 0;
      child_requisition.height = 0;
    }

  requisition->width  += child_requisition.width;
  requisition->height += child_requisition.height;
}

static void
gimp_overlay_frame_size_allocate (GtkWidget     *widget,
                                  GtkAllocation *allocation)
{
  GtkContainer  *container = GTK_CONTAINER (widget);
  GtkWidget     *child     = gtk_bin_get_child (GTK_BIN (widget));
  GtkAllocation  child_allocation;
  gint           border_width;

  gtk_widget_set_allocation (widget, allocation);

  border_width = gtk_container_get_border_width (container);

  if (child && gtk_widget_get_visible (child))
    {
      child_allocation.x      = allocation->x + border_width;
      child_allocation.y      = allocation->y + border_width;
      child_allocation.width  = MAX (allocation->width  - 2 * border_width, 0);
      child_allocation.height = MAX (allocation->height - 2 * border_width, 0);

      gtk_widget_size_allocate (child, &child_allocation);
    }
}

static gboolean
gimp_overlay_frame_expose (GtkWidget      *widget,
                           GdkEventExpose *eevent)
{
  cairo_t       *cr = gdk_cairo_create (gtk_widget_get_window (widget));
  GtkStyle      *style;
  GtkAllocation  allocation;
  gint           border_width;

  style = gtk_widget_get_style (widget);
  gtk_widget_get_allocation (widget, &allocation);

  border_width = gtk_container_get_border_width (GTK_CONTAINER (widget));

  cairo_set_operator (cr, CAIRO_OPERATOR_CLEAR);
  gdk_cairo_region (cr, eevent->region);
  cairo_clip_preserve (cr);
  cairo_fill (cr);

  cairo_set_operator (cr, CAIRO_OPERATOR_OVER);
  gdk_cairo_set_source_color (cr, &style->bg[GTK_STATE_NORMAL]);

#define TO_RAD(deg) (deg * (G_PI / 180.0))

  cairo_arc (cr,
             border_width,
             border_width,
             border_width,
             TO_RAD (180),
             TO_RAD (270));
  cairo_line_to (cr,
                 allocation.width - border_width,
                 0);

  cairo_arc (cr,
             allocation.width - border_width,
             border_width,
             border_width,
             TO_RAD (270),
             TO_RAD (0));
  cairo_line_to (cr,
                 allocation.width,
                 allocation.height - border_width);

  cairo_arc (cr,
             allocation.width  - border_width,
             allocation.height - border_width,
             border_width,
             TO_RAD (0),
             TO_RAD (90));
  cairo_line_to (cr,
                 border_width,
                 allocation.height);

  cairo_arc (cr,
             border_width,
             allocation.height - border_width,
             border_width,
             TO_RAD (90),
             TO_RAD (180));
  cairo_close_path (cr);

  cairo_fill (cr);

  cairo_destroy (cr);

  return GTK_WIDGET_CLASS (parent_class)->expose_event (widget, eevent);
}

GtkWidget *
gimp_overlay_frame_new (void)
{
  return g_object_new (GIMP_TYPE_OVERLAY_FRAME, NULL);
}