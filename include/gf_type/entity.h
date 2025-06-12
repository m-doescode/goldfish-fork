/**
 * @file gf_type/entity.h
 * @~english
 * @brief Type definitions related to entities
 */

#ifndef __GF_TYPE_ENTITY_H__
#define __GF_TYPE_ENTITY_H__

#include <gf_type/math.h>
#include <gf_pre.h>
#include <gf_macro.h>

#define gf_entity_t struct gf_entity
gf_entity_t;

#ifdef GF_EXPOSE_ENTITY
#define gf_entity_class_t struct gf_entity_class
gf_entity_class_t;

// Flags
#define GF_ENTITY_LIVING		(1L << 0) // Entity has health, and should die when it reaches 0
#define GF_ENTITY_HAS_PHYSICS		(1L << 1) // Entity has physics
#define GF_ENTITY_STATIC_PROP		(1L << 1) // Entity has stationary physics (must also add GF_ENTITY_HAS_PHYSICS)

enum GF_ENTITY_EVENT_TYPE {
	GF_ENTITY_CREATED, // The entity is added to the world
	GF_ENTITY_REMOVED, // The entity is removed from the world
	GF_ENTITY_DIED, // The entity's health has reached 0 (if it is a living entity), or been killed via kill command

	GF_ENTITY_PROPERTY_UPDATED, // Property has been updated, arg == struct { const char* prop_name, void* prev_value };

	GF_ENTITY_PHYS_COLLISION, // Entity has collided with another entity, arg == gf_entity_t* colliding entity
};

#define GF_ENTITY_HANDLER_DEFAULT		(void*)-1 // Event not handled, use default behavior

typedef void* (*gf_entity_event_handler_t)(int event_type, void* arg);

GF_DECLARE_TYPE(entity_class, {
	const char* class_name;
	gf_entity_event_handler_t event_handler;
	int flags;
});

#else
typedef void gf_entity_class_t;
#endif

GF_DECLARE_TYPE(entity, {
	const gf_entity_class_t* class;
	const char* name;
	gf_math_vector_t pos;
	gf_math_vector_t angles;
})

#endif
