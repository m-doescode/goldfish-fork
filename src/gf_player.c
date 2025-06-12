#include "gf_type/entity.h"
#define GF_EXPOSE_ENTITY

#include <gf_pre.h>

/* Interface */
#include <gf_entity.h>

void* gf_player_event_handler(int event_type, void* arg) {
	switch (event_type) {
	case GF_ENTITY_DIED:
		// Spawn ragdoll, etc.
		return 0;
	}
	return GF_ENTITY_HANDLER_DEFAULT; // NYI
}

const gf_entity_class_t gf_player_class = {
	.class_name = "player",
	.event_handler = &gf_player_event_handler,
	.flags = GF_ENTITY_LIVING | GF_ENTITY_HAS_PHYSICS
};