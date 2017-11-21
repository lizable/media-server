#include "file-reader.h"
#include "file-writer.h"
#include "mov-internal.h"
#include <assert.h>
#include <stdlib.h>
#include <errno.h>

// extra_data: ISO/IEC 14496-15:2017 HEVCDecoderConfigurationRecord

int mov_read_hvcc(struct mov_t* mov, const struct mov_box_t* box)
{
	struct mov_track_t* track = mov->track;
	if (track->extra_data_size < box->size)
	{
		void* p = realloc(track->extra_data, (size_t)box->size);
		if (NULL == p) return ENOMEM;
		track->extra_data = p;
	}

	file_reader_read(mov->fp, track->extra_data, (size_t)box->size);
	track->extra_data_size = (size_t)box->size;
	return file_reader_error(mov->fp);
}

size_t mov_write_hvcc(const struct mov_t* mov)
{
	const struct mov_track_t* track = mov->track;
	file_writer_wb32(mov->fp, track->extra_data_size + 8); /* size */
	file_writer_write(mov->fp, "hvcC", 4);
	if (track->extra_data_size > 0)
		file_writer_write(mov->fp, track->extra_data, track->extra_data_size);
	return track->extra_data_size + 8;
}
