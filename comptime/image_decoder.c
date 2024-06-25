/*
 * Copyright © 2024 wr7
 *
 * A compile-time tool that uses ffmpeg to convert images into C headers
 *
 * Usage: image_decoder <input_file> <output_file>
 *
 * The output format is packed RGBA (32 bpp). The variable names are based on 
 * the input file name. An input file `foo@BAR.png` would have the following 
 * constants:
 *
 * int FOO_BAR_PNG_WIDTH  = ...;
 * int FOO_BAR_PNG_HEIGHT = ...;
 * unsigned char FOO_BAR_PNG[] = {...};
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>
#include <string.h>

#include <libswscale/swscale.h>

#include <libavcodec/avcodec.h>

#include <libavformat/avformat.h>

#include <libavutil/avutil.h>

static noreturn void error(const char *const msg) {
	fprintf(stderr, "%s: %s\n", __FILE__, msg);
	exit(1);
}

static noreturn void errno_error(const char *const msg) {
	fprintf(stderr, "%s: ", __FILE__);
	perror(msg);
	exit(1);
}

static int handleAvError(int possible_error) {
	if(possible_error < 0) {
		static char msg[128] = "Failed to get ffmpeg error";
		av_strerror(possible_error, msg, sizeof(msg));

		fprintf(stderr, "%s: An ffmpeg error has occurred: %s\n", __FILE__, msg);
		exit(1);
	}

	return possible_error;
}

static int handleWriteError(int possible_error) {
	if(possible_error < 0) {
		errno_error("Failed to write to output file");
	}

	return possible_error;
}

/// Converts the frame to the correct pixel format (RGBA) 32bpp
static void convertFrame(AVFrame **frame) {
	if((*frame)->format == AV_PIX_FMT_RGBA) {
		return;
	}

	AVFrame *src = *frame;
	AVFrame *dst = av_frame_alloc();

	dst->height = src->height;
	dst->width = dst->width;

	struct SwsContext *ctx = sws_getContext(src->width, src->height, src->format, src->width, src->height, AV_PIX_FMT_RGBA, 0, NULL, NULL, NULL);
	if(ctx == NULL) {
		error("Failed to convert pixel format");
	}
	
	handleAvError(
		sws_scale_frame(ctx, dst, src)
	);

	sws_freeContext(ctx);
	av_frame_free(&src);

	*frame = dst;
}

/// Reads the input file into a frame
static AVFrame *readFrame(const char *const file_name) {
	// Open input file //

	AVFormatContext *fmt_ctx = avformat_alloc_context();
	handleAvError(
		avformat_open_input(&fmt_ctx, file_name, NULL, NULL)
	);

	handleAvError(
		avformat_find_stream_info(fmt_ctx, NULL)
	);

	if(
		fmt_ctx->nb_streams != 1 || 
		fmt_ctx->streams[0]->codecpar->codec_type != AVMEDIA_TYPE_VIDEO ||
		fmt_ctx->streams[0]->nb_frames > 1
	) {
		error("Expected input to be an image");
	}

	// Decode input file //

	const AVCodecParameters *const codec_par = fmt_ctx->streams[0]->codecpar;
	const AVCodec *const codec = avcodec_find_decoder(codec_par->codec_id);
	if(codec == NULL) {
		error("Failed to find decoder for image format");
	}

	AVCodecContext *codec_ctx = avcodec_alloc_context3(codec);
	if(codec_ctx == NULL) {
		error("Failed to allocate codec context");
	}

	handleAvError(
		avcodec_parameters_to_context(codec_ctx, codec_par)
	);

	handleAvError(
		avcodec_open2(codec_ctx, codec, NULL)
	);

	AVPacket *packet = av_packet_alloc();
	AVFrame *frame = av_frame_alloc();

	handleAvError(
		av_read_frame(fmt_ctx, packet)
	);
	handleAvError(
		avcodec_send_packet(codec_ctx, packet)
	);
	handleAvError(
		avcodec_receive_frame(codec_ctx, frame)
	);

	convertFrame(&frame);

	av_packet_free(&packet);
	avcodec_free_context(&codec_ctx);
	avformat_free_context(fmt_ctx);

	return frame;
}

/**
 * Gets the output variable name based on the input file name
 * 
 * ie `foo bAr.png` -> `FOO_BAR_PNG`
 */
static char *output_var_name(const char *const input_name) {
	size_t input_len = strlen(input_name);
	char *out = calloc(input_len + 1, sizeof(*out));

	for(size_t i = 0; i < input_len; i++) {
		char input_char = input_name[i];

		if((input_char > 'A' && input_char < 'Z') ||
		   (input_char > 'a' && input_char < 'z')
		) {
			char uppercase = input_char &~ ('a' ^ 'A');
			out[i] = uppercase;
		} else {
			out[i] = '_';
		}
	}

	return out;
}

int main(const int argc, const char *const *const args) {
	// Read parameters //
	if(argc < 3) {
		error("Expected input and output file name");
	}

	const char *const input_name = args[1];
	const char *const output_name = args[2];

	AVFrame *frame = readFrame(input_name);

	FILE *out = fopen(output_name, "w");
	if(out == NULL) {
		errno_error("Failed to open input file");
	}

	char *var_name = output_var_name(input_name);

	handleWriteError(
		fprintf(
			out, "int %s_WIDTH	= %d;\n"
			     "int %s_HEIGHT = %d;\n"
			     "unsigned char %s[] = {", 
			      var_name, frame->width,
			      var_name, frame->height,
			      var_name
		)
	);

	size_t image_size = frame->width * frame->height * 4;

	for(size_t i = 0; i < image_size; i++) {
		unsigned char byte = frame->buf[0]->data[i];

		handleWriteError(
			fprintf(out, "%u,", byte)
		);
	}

	handleWriteError(
		fprintf(out, "};\n")
	);

	fclose(out);
	free(var_name);
	av_frame_free(&frame);

	return 0;
}
