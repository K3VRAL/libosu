#include "file/beatmap.h"

void ofb_metadata_setfromstring(Metadata *metadata, char *key_value_pair) {
	char *token = strtok(key_value_pair, ":");
	if (token == NULL) {
		return;
	}

	char *key = strdup(token);
	char *value = strtok(NULL, "\0");
	if (value == NULL) {
		free(key);
		return;
	}

	if (*(value + 0) == ' ') {
		value++;
	}
	if (strcmp("Title", key) == 0) {
		metadata->title = strdup(value);
	} else if (strcmp("TitleUnicode", key) == 0) {
		metadata->title_unicode = strdup(value);
	} else if (strcmp("Artist", key) == 0) {
		metadata->artist = strdup(value);
	} else if (strcmp("ArtistUnicode", key) == 0) {
		metadata->artist_unicode = strdup(value);
	} else if (strcmp("Creator", key) == 0) {
		metadata->creator = strdup(value);
	} else if (strcmp("Version", key) == 0) {
		metadata->version = strdup(value);
	} else if (strcmp("Source", key) == 0) {
		metadata->source = strdup(value);
	} else if (strcmp("Tags", key) == 0) {
		metadata->tags = calloc(1, sizeof(*metadata->tags));
		token = strtok(value, " ");
		while (token != NULL) {
			metadata->tags = realloc(metadata->tags, (metadata->num_tag + 1) * sizeof(*metadata->tags));
			*(metadata->tags + metadata->num_tag++) = strdup(token);
			token = strtok(NULL, " ");
		}
	} else if (strcmp("BeatmapID", key) == 0) {
		metadata->beatmap_id = (int) strtol(value, NULL, 10);
	} else if (strcmp("BeatmapSetID", key) == 0) {
		metadata->beatmap_set_id = (int) strtol(value, NULL, 10);
	}
	free(key);
}

void ofb_metadata_tostring(char **output, Metadata metadata) {
	ComparingMetadata data[] = {
		{
			.name = "Title",
			.info.cp = metadata.title,
			.type = m_cp,
		},
		{
			.name = "TitleUnicode",
			.info.cp = metadata.title_unicode,
			.type = m_cp,
		},
		{
			.name = "Artist",
			.info.cp = metadata.artist,
			.type = m_cp,
		},
		{
			.name = "ArtistUnicode",
			.info.cp = metadata.artist_unicode,
			.type = m_cp,
		},
		{
			.name = "Creator",
			.info.cp = metadata.creator,
			.type = m_cp,
		},
		{
			.name = "Version",
			.info.cp = metadata.version,
			.type = m_cp,
		},
		{
			.name = "Source",
			.info.cp = metadata.source,
			.type = m_cp,
		},
		{
			.name = "Tags",
			.info.s.cpp = metadata.tags,
			.info.s.ui = metadata.num_tag,
			.type = m_s,
		},
		{
			.name = "BeatmapID",
			.info.n = metadata.beatmap_id,
			.type = m_n,
		},
		{
			.name = "BeatmapSetID",
			.info.n = metadata.beatmap_set_id,
			.type = m_n,
		},
	};
	*output = ou_comparing_metadata(data, 10);
}