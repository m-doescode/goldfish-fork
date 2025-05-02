#define GF_EXPOSE_RESOURCE

#include <gf_pre.h>

/* External library */
#include <stb_ds.h>
#include <zlib.h>

/* Interface */
#include <gf_resource.h>

/* Engine */
#include <gf_core.h>
#include <gf_log.h>

/* Standard */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define CHUNK 32767

gf_resource_t* gf_resource_create(gf_engine_t* engine, const char* path) {
	FILE*	       f;
	gf_resource_t* resource = malloc(sizeof(*resource));

	memset(resource, 0, sizeof(*resource));
	resource->engine  = engine;
	resource->entries = NULL;
	sh_new_strdup(resource->entries);

	if(path == NULL) {
		gf_log_function(engine, "Created empty resource", "");
		return resource;
	}
	f = fopen(path, "rb");

	if(f == NULL) {
		gf_log_function(engine, "Failed to create resource", "");
		gf_resource_destroy(resource);
		return NULL;
	}

	gf_log_function(engine, "Created resource", "");

	while(1) {
		char		    filename[128];
		unsigned char	    n;
		size_t		    sz = 0;
		int		    i;
		gf_resource_entry_t e;
		fread(&filename, 128, 1, f);
		if(filename[0] == 0) break;

		for(i = 0; i < 4; i++) {
			fread(&n, 1, 1, f);
			sz = sz << 8;
			sz = sz | n;
		}

		e.key	 = (char*)&filename[0];
		e.size	 = sz;
		e.ogsize = 0;

		e.cache	     = NULL;
		e.compressed = malloc(sz);
		fread(e.compressed, sz, 1, f);
		shputs(resource->entries, e);

		gf_log_function(engine, "%s: Compressed to %lu bytes", filename, (unsigned long)sz);
	}
	fclose(f);

	return resource;
}

int gf_resource_get(gf_resource_t* resource, const char* name, void** data, size_t* size) {
	int		     ind = shgeti(resource->entries, name);
	gf_resource_entry_t* e;
	if(ind == -1) return -1;

	*size = 0;
	*data = NULL;

	e = &resource->entries[ind];
	if(e->cache == NULL) {
		unsigned char  out[CHUNK];
		z_stream       stream;
		int	       ret;
		int	       have;
		size_t	       dsz = e->size;
		unsigned char* ptr = e->compressed;

		e->ogsize = 0;
		e->cache  = NULL;

		stream.zalloc	= Z_NULL;
		stream.zfree	= Z_NULL;
		stream.opaque	= Z_NULL;
		stream.avail_in = 0;
		stream.next_in	= Z_NULL;
		inflateInit(&stream);
		gf_log_function(resource->engine, "%s: Not cached, decompressing", name);
		do {
			size_t rds	= CHUNK > dsz ? dsz : CHUNK;
			stream.avail_in = rds;
			if(stream.avail_in == 0) break;
			stream.next_in = ptr;
			do {
				stream.avail_out = CHUNK;
				stream.next_out	 = out;
				ret		 = inflate(&stream, Z_NO_FLUSH);
				switch(ret) {
				case Z_NEED_DICT:
				case Z_DATA_ERROR:
				case Z_MEM_ERROR:
					inflateEnd(&stream);
					if(e->cache != NULL) {
						free(e->cache);
						e->cache = NULL;
					}
					e->ogsize = 0;
					return -1;
				}
				have = CHUNK - stream.avail_out;

				if(e->cache == NULL) {
					e->cache = malloc(have);
					memcpy(e->cache, out, have);
				} else {
					unsigned char* old = e->cache;
					e->cache	   = malloc(e->ogsize + have);

					memcpy(e->cache, old, e->ogsize);
					memcpy(e->cache + e->ogsize, out, have);
					free(old);
				}

				e->ogsize += have;
			} while(stream.avail_out == 0);
			ptr += rds;
			dsz -= rds;
		} while(ret != Z_STREAM_END);
		inflateEnd(&stream);
		gf_log_function(resource->engine, "%s: Compression rate is %.2f%%", name, (double)e->ogsize / e->size * 100);
	} else {
		gf_log_function(resource->engine, "%s: Using cache", name);
	}

	if(e->cache != NULL) {
		*data = malloc(e->ogsize);
		*size = e->ogsize;

		memcpy(*data, e->cache, *size);
		return 0;
	}

	return -1;
}

void gf_resource_add(gf_resource_t* resource, const char* name, void* data, size_t size) {
	gf_resource_entry_t e;
	if(size == 0) return;

	e.key	 = (char*)name;
	e.size	 = 0;
	e.ogsize = size;

	e.compressed = NULL;
	e.cache	     = malloc(size);
	memcpy(e.cache, data, size);
	shputs(resource->entries, e);
}

void gf_resource_write(gf_resource_t* resource, const char* path, int progress) {
	int	      i;
	FILE*	      f = fopen(path, "wb");
	unsigned char fn[128];
	if(f == NULL) return;

	for(i = 0; i < shlen(resource->entries); i++) {
		gf_resource_entry_t* e = &resource->entries[i];
		int		     j;
		size_t		     sz	  = 0;
		size_t		     sz2  = 0;
		char*		     data = NULL;

		if(progress && e->size == 0) {
			printf("%s", e->key);
			fflush(stdout);
		}

		memset(fn, 0, 128);
		strcpy(fn, e->key);
		fwrite(fn, 128, 1, f);

		if(e->size != 0) {
			sz = e->size;
		} else {
			unsigned char  out[CHUNK];
			int	       flush;
			z_stream       stream;
			size_t	       dsz = e->ogsize;
			unsigned char* ptr = e->cache;
			int	       have;

			stream.zalloc = Z_NULL;
			stream.zfree  = Z_NULL;
			stream.opaque = Z_NULL;
			deflateInit(&stream, Z_DEFAULT_COMPRESSION);

			do {
				size_t wts	= CHUNK > dsz ? dsz : CHUNK;
				stream.avail_in = wts;
				stream.next_in	= ptr;
				flush		= CHUNK >= dsz ? Z_FINISH : Z_NO_FLUSH;
				do {
					stream.avail_out = CHUNK;
					stream.next_out	 = out;
					deflate(&stream, flush);
					have = CHUNK - stream.avail_out;

					if(data == NULL) {
						data = malloc(have);
						memcpy(data, out, have);
					} else {
						unsigned char* old = data;
						data		   = malloc(sz + have);
						memcpy(data, old, sz);
						memcpy(data + sz, out, have);
						free(old);
					}

					sz += have;

					if(progress) {
						printf(".");
						fflush(stdout);
					}
				} while(stream.avail_out == 0);
				ptr += wts;
				dsz -= wts;
			} while(flush != Z_FINISH);
			deflateEnd(&stream);
		}

		sz2 = sz;
		for(j = 0; j < 4; j++) {
			unsigned char c = 0;
			c		= ((sz >> 24) & 0xff);
			fwrite(&c, 1, 1, f);

			sz = sz << 8;
		}
		sz = sz2;

		if(e->size != 0) {
			fwrite(e->compressed, sz, 1, f);
		} else if(data != NULL) {
			fwrite(data, sz, 1, f);
			e->compressed = data;
			e->size	      = sz;
		}

		if(progress && data != NULL) {
			printf(" %.2f%%\n", (double)e->ogsize / e->size * 100);
		}
	}

	memset(fn, 0, 128);
	fwrite(fn, 128, 1, f);
	fclose(f);
}

void gf_resource_destroy(gf_resource_t* resource) {
	if(resource->entries != NULL) {
		int i;
		for(i = 0; i < shlen(resource->entries); i++) {
			gf_resource_entry_t* e = &resource->entries[i];
			if(e->cache != NULL) free(e->cache);
			if(e->compressed != NULL) free(e->compressed);
		}
		shfree(resource->entries);
	}
	gf_log_function(resource->engine, "Destroyed resource", "");
	free(resource);
}
