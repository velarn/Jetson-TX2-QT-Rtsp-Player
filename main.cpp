#include "mainwindow.h"
#include <QApplication>

#include <gst/gst.h>
#include <video/videooverlay.h>
#include <QTimer>
#include <QWidget>

//int global_cam_winid;
//GstBusSyncReply bus_sync_handler (GstBus * bus, GstMessage * message, gpointer data)
//{
//    // ignore anything but 'prepare-xwindow-id' element messages
//    if (GST_MESSAGE_TYPE (message) != GST_MESSAGE_ELEMENT){
//        //printf("\nerr1 : %d\n",GST_MESSAGE_TYPE (message));
//        return GST_BUS_PASS;
//    }
//    if (!gst_structure_has_name (message->structure, "prepare-xwindow-id")){
//        //printf("\nerr2\n");
//        return GST_BUS_PASS;
//    }

//    if (global_cam_winid != 0) {
//        GstXOverlay *xoverlay;
//        // GST_MESSAGE_SRC (message) will be the video sink element
//        xoverlay = GST_X_OVERLAY (GST_MESSAGE_SRC (message));
//        gst_x_overlay_set_window_handle (xoverlay, global_cam_winid);
//        printf("\nerr3\n");
//    } else {
//        g_warning ("Should have obtained video_window_xid by now!");
//    }

//    gst_message_unref (message);
//    return GST_BUS_DROP;
//}


static void on_pad_added(GstElement *element, GstPad *pad, gpointer data)
{
    gchar *name;
    name = gst_pad_get_name(pad);
    g_print("A new pad %s was created.\n", name);
    g_free(name);

    GstPad *sinkpad;
    GstElement *downstream = (GstElement *) data;

    sinkpad = gst_element_get_static_pad(downstream, "sink");

    gst_pad_link (pad, sinkpad);

    gst_object_unref(sinkpad);
}


#define th264 1


int main(int argc, char *argv[])
{
    g_print("start main.\n");

    if (!g_thread_supported ())
        g_thread_init (NULL);

    gst_init (&argc, &argv);

    QApplication app(argc, argv);
    app.connect(&app, SIGNAL(lastWindowClosed()), &app, SLOT(quit ()));

    GstElement *pipeline,*src,*decode,*video_convert,*sink;

    pipeline = gst_pipeline_new ("xvoverlay");

    if(!th264){

        src = gst_element_factory_make ("videotestsrc", NULL);
        sink = gst_element_factory_make ("xvimagesink", NULL);
        gst_bin_add_many (GST_BIN (pipeline), src, sink, NULL);
        gst_element_link (src, sink);
    }
    else{

        src = gst_element_factory_make ("rtspsrc", "source");
        g_object_set(G_OBJECT(src),"location","rtsp://192.168.0.80/h264",NULL);

        decode = gst_element_factory_make ("decodebin", "decoder");
        video_convert = gst_element_factory_make ("videoconvert", "convert");
        sink = gst_element_factory_make ("xvimagesink", "sink");


        gst_bin_add_many (GST_BIN (pipeline), src,decode ,video_convert,sink, NULL);

        g_signal_connect(src, "pad-added", G_CALLBACK(on_pad_added), decode);
        g_signal_connect(decode, "pad-added", G_CALLBACK(on_pad_added), video_convert);
        g_signal_connect(video_convert, "pad-added", G_CALLBACK(on_pad_added), sink);

//        For a long time ,I used the method "gst element lin many",but i got nothing in qt window
//        When I change this method to "gst element link",it works fine
//        gst_element_link_many(src,decode,video_convert,sink,NULL);
        gst_element_link(src,decode);
        gst_element_link(decode,video_convert);
        gst_element_link(video_convert,sink);

    }

    // prepare the ui

    MainWindow window;
    window.resize(640, 480);
    window.show();


    WId xwinid = window.winId();

    gst_video_overlay_set_window_handle (GST_VIDEO_OVERLAY (sink), xwinid);

    // run the pipeline

    GstStateChangeReturn sret = gst_element_set_state (pipeline,GST_STATE_PLAYING);

    if (sret == GST_STATE_CHANGE_FAILURE) {
      gst_element_set_state (pipeline, GST_STATE_NULL);
      gst_object_unref (pipeline);
      // Exit application
      QTimer::singleShot(0, QApplication::activeWindow(), SLOT(quit()));
    }


    int ret = app.exec();

    window.hide();
    gst_element_set_state (pipeline, GST_STATE_NULL);
    gst_object_unref (pipeline);

    return ret;

}


