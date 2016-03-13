#include <math.h>

#include <libavutil/opt.h>
#include <libavcodec/avcodec.h>
#include <libavutil/channel_layout.h>
#include <libavutil/common.h>
#include <libavutil/imgutils.h>
#include <libavutil/mathematics.h>
#include <libavutil/samplefmt.h>

/*
 * Video encoding example
 */
static void video_encode_example(const char *filename) {
  AVCodec* codec;
  /* find the mpeg1 video encoder */
  codec = avcodec_find_encoder(AV_CODEC_ID_VP8);
  if (!codec) {
    fprintf(stderr, "Codec not found\n");
    exit(1);
  }

  AVCodecContext* av_codec_context = avcodec_alloc_context3(codec);
  if (!av_codec_context) {
    fprintf(stderr, "Could not allocate video codec context\n");
    exit(1);
  }

  int got_output;
  AVFrame *frame;
  AVPacket pkt;
  uint8_t endcode[] = { 0, 0, 1, 0xb7 };

  printf("Encode video file %s\n", filename);

  /* put sample parameters */
  av_codec_context->bit_rate = 400000;
  /* resolution must be a multiple of two */
  av_codec_context->width = 352;
  av_codec_context->height = 288;
  /* frames per second */
  av_codec_context->time_base = (AVRational){1,25};
  /* emit one intra frame every ten frames
   * check frame pict_type before passing frame
   * to encoder, if frame->pict_type is AV_PICTURE_TYPE_I
   * then gop_size is ignored and the output of encoder
   * will always be I frame irrespective to gop_size
   */
  av_codec_context->gop_size = 10;
  av_codec_context->max_b_frames = 1;
  av_codec_context->pix_fmt = AV_PIX_FMT_YUV420P;

  /* open it */
  if (avcodec_open2(av_codec_context, codec, NULL) < 0) {
    fprintf(stderr, "Could not open codec\n");
    exit(1);
  }

  FILE* f = fopen(filename, "wb");
  if (!f) {
    fprintf(stderr, "Could not open %s\n", filename);
    exit(1);
  }

  frame = av_frame_alloc();
  if (!frame) {
    fprintf(stderr, "Could not allocate video frame\n");
    exit(1);
  }
  frame->format = av_codec_context->pix_fmt;
  frame->width  = av_codec_context->width;
  frame->height = av_codec_context->height;

  /* the image can be allocated by any means and av_image_alloc() is
   * just the most convenient way if av_malloc() is to be used */
  if (av_image_alloc(frame->data,
                     frame->linesize,
                     av_codec_context->width,
                     av_codec_context->height,
                     av_codec_context->pix_fmt,
                     32) < 0) {
    fprintf(stderr, "Could not allocate raw picture buffer\n");
    exit(1);
  }

  /* encode 1 second of video */
  for (int i = 0; i < 250; i++) {
    av_init_packet(&pkt);
    pkt.data = NULL;  // packet data will be allocated by the encoder
    pkt.size = 0;

    fflush(stdout);
    /* prepare a dummy image */
    /* Y */
    for (int y = 0; y < av_codec_context->height; y++) {
      for (int x = 0; x < av_codec_context->width; x++) {
        frame->data[0][y * frame->linesize[0] + x] = x + y + i * 3;
      }
    }

    /* Cb and Cr */
    for (int y = 0; y < av_codec_context->height/2; y++) {
      for (int x = 0; x < av_codec_context->width/2; x++) {
        frame->data[1][y * frame->linesize[1] + x] = 128 + y + i * 2;
        frame->data[2][y * frame->linesize[2] + x] = 64 + x + i * 5;
      }
    }

    frame->pts = i;

    /* encode the image */
    if (0 > avcodec_encode_video2(av_codec_context, &pkt, frame, &got_output)) {
      fprintf(stderr, "Error encoding frame\n");
      exit(1);
    }

    if (got_output) {
      printf("Write frame %3d (size=%5d)\n", i, pkt.size);
      fwrite(pkt.data, 1, pkt.size, f);
      av_packet_unref(&pkt);
    }
  }

  /* get the delayed frames */
  while (1) {
    int got_output;
    if (avcodec_encode_video2(av_codec_context, &pkt, NULL, &got_output) < 0) {
      fprintf(stderr, "Error encoding frame\n");
      exit(1);
    }

    if (got_output) {
      fwrite(pkt.data, 1, pkt.size, f);
      av_packet_unref(&pkt);
    } else {
      break;
    }
  }

  /* add sequence end code to have a real mpeg file */
  fwrite(endcode, 1, sizeof(endcode), f);
  fclose(f);

  avcodec_close(av_codec_context);
  av_free(av_codec_context);
  av_freep(&frame->data[0]);
  av_frame_free(&frame);
}

int main(int argc, char **argv) {
  /* register all the codecs */
  avcodec_register_all();

  video_encode_example("test.webm");

  return 0;
}
