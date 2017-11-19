#ifndef _SUI_TYPE_H
#define _SUI_TYPE_H
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <chopsui/scalars.h>

struct sui_node;

// TODO: consider passing an errors_t to attr and child

struct sui_type_impl {
	/**
	 * Invoked when a new node is created of this type.
	 */
	void (*init)(struct sui_node *node);
	/**
	 * Invoked when this node is destroyed. Don't free the node itself, just any
	 * state you've associated with it.
	 */
	void (*free)(struct sui_node *node);
	/**
	 * Invoked when an attribute is set on this node. Return false to prevent
	 * the change from being applied.
	 */
	bool (*attr)(struct sui_node *node, const char *key,
			const struct sui_scalar *value);
	/**
	 * Return the valid scalar types for this attribute, ORed together. attr is
	 * never called with a value that does not match this spec.
	 */
	uint64_t (*attr_spec)(struct sui_node *node, const char *key);
	/**
	 * Provide the default value for this attribute. Set the scalar type to
	 * SCALAR_EMPTY to indicate an invalid attribute.
	 */
	void (*attr_default)(const char *key, struct sui_scalar *value);
	/**
	 * Invoked when a child is added to this node. Return false to prevent the
	 * child from being added and raise an error.
	 */
	bool (*child)(struct sui_node *node, struct sui_node *child);
	/**
	 * Invoked when a child is removed from this node.
	 */
	void (*child_removed)(struct sui_node *node, struct sui_node *child);
};

/**
 * Adds a sui_type_impl that is automatically added to new nodes created of the
 * specified type. The new impl is not added to existing nodes of this type.
 */
void type_impl_register(const char *type, const struct sui_type_impl *impl);

/**
 * Registers a type with many sui_type_impls. The last argument should be NULL.
 */
void type_impls_register(const char *type, ...);

// TODO: Consider just making them pass this instead of letting them get fancy
// in the sui_type_impl
/**
 * Helper type for simple implementations of type impl functions.
 */
struct attr_spec {
	char *key;
	uint64_t spec;
	struct sui_scalar value;
};

/**
 * Implements sui_type_impl.attr_spec using the given attr_specs.
 */
#define attr_spec_attr_spec(spec, key) \
	_attr_spec_attr_spec(spec, sizeof(spec) / sizeof((*spec)[0]), key)
uint64_t _attr_spec_attr_spec(const struct attr_spec (*spec)[], size_t len,
		const char *key);
/**
 * Implements sui_type_impl.attr_default using the given attr_specs.
 */
#define attr_spec_attr_default(spec, key, value) \
	_attr_spec_attr_default(spec, sizeof(spec) / sizeof(*spec), key, value)
void _attr_spec_attr_default(const struct attr_spec (*spec)[], size_t len,
		const char *key, struct sui_scalar *value);

#endif
