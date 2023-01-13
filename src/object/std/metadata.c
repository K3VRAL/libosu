#include "object/std.h"

void oos_metadata_init(Metadata *metadata) {
	metadata->title = NULL;
	metadata->title_unicode = NULL;
	metadata->artist = NULL;
	metadata->artist_unicode = NULL;
	metadata->creator = NULL;
	metadata->version = NULL;
	metadata->source = NULL;
	metadata->tags = NULL;
	metadata->num_tag = 0;
	metadata->beatmap_id = -1;
	metadata->beatmap_set_id = -1;
}

void oos_metadata_free(Metadata metadata) {
	if (metadata.title != NULL) {
		free(metadata.title);
	}
	if (metadata.title_unicode != NULL) {
		free(metadata.title_unicode);
	}
	if (metadata.artist != NULL) {
		free(metadata.artist);
	}
	if (metadata.artist_unicode != NULL) {
		free(metadata.artist_unicode);
	}
	if (metadata.creator != NULL) {
		free(metadata.creator);
	}
	if (metadata.version != NULL) {
		free(metadata.version);
	}
	if (metadata.source != NULL) {
		free(metadata.source);
	}
	if (metadata.tags != NULL) {
		for (int i = 0; i < metadata.num_tag; i++) {
			free(*(metadata.tags + i));
		}
		free(metadata.tags);
	}
}