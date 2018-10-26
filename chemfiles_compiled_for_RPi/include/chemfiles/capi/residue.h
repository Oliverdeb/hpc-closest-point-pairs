// Chemfiles, a modern library for chemistry file reading and writing
// Copyright (C) Guillaume Fraux and contributors -- BSD license

#ifndef CHEMFILES_CHFL_RESIDUE_H
#define CHEMFILES_CHFL_RESIDUE_H

#include "chemfiles/capi/types.h"
#ifdef __cplusplus
extern "C" {
#endif

/// Create a new residue with the given `name`.
///
/// The caller of this function should free the allocated memory using
/// `chfl_residue_free`.
///
/// @example{tests/capi/doc/chfl_residue/chfl_residue.c}
/// @return A pointer to the residue, or NULL in case of error.
///         You can use `chfl_last_error` to learn about the error.
CHFL_EXPORT CHFL_RESIDUE* chfl_residue(const char* name);

/// Create a new residue with the given `name` and residue identifier `resid`.
///
/// The caller of this function should free the allocated memory using
/// `chfl_residue_free`.
///
/// @example{tests/capi/doc/chfl_residue/with_id.c}
/// @return A pointer to the residue, or NULL in case of error.
///         You can use `chfl_last_error` to learn about the error.
CHFL_EXPORT CHFL_RESIDUE* chfl_residue_with_id(const char* name, uint64_t resid);

/// Get access to the residue at index `i` in a `topology`.
///
/// If `i` is bigger than the result of `chfl_topology_residues_count`, this
/// function will return `NULL`.
///
/// The residue index in the topology is not always the same as the residue
/// `id`.
///
/// The `topology` will be kept alive, even if `chfl_topology_free` is called,
/// until `chfl_residue_free` is also called on the pointer returned by this
/// function.
///
/// The pointer returned by this function points directly inside the topology,
/// and will be invalidated if any of the following function is called on the
/// topology:
///
/// - `chfl_topology_add_residue`
///
/// Calling any function on an invalidated pointer is undefined behavior.
/// Even if the pointer if invalidated, it stills needs to be released with
/// `chfl_residue_free`.
///
/// @example{tests/capi/doc/chfl_residue/from_topology.c}
/// @return A pointer to the residue, or NULL in case of error.
///         You can use `chfl_last_error` to learn about the error.
CHFL_EXPORT const CHFL_RESIDUE* chfl_residue_from_topology(
    const CHFL_TOPOLOGY* topology, uint64_t i
);

/// Get access to the residue containing the atom at index `i` in the
/// `topology`.
///
/// This function will return `NULL` if the atom is not in a residue, or if the
/// index `i` is bigger than `chfl_topology_atoms_count`.
///
/// The `topology` will be kept alive, even if `chfl_topology_free` is called,
/// until `chfl_residue_free` is also called on the pointer returned by this
/// function.
///
/// The pointer returned by this function points directly inside the topology,
/// and will be invalidated if any of the following function is called on the
/// topology:
///
/// - `chfl_topology_add_residue`
///
/// Calling any function on an invalidated pointer is undefined behavior.
/// Even if the pointer if invalidated, it stills needs to be released with
/// `chfl_residue_free`.
///
/// @example{tests/capi/doc/chfl_residue/for_atom.c}
/// @return A pointer to the residue, or NULL in case of error.
///         You can use `chfl_last_error` to learn about the error.
CHFL_EXPORT const CHFL_RESIDUE* chfl_residue_for_atom(
    const CHFL_TOPOLOGY* topology, uint64_t i
);

/// Get a copy of a `residue`.
///
/// The caller of this function should free the associated memory using
/// `chfl_residue_free`.
///
/// @example{tests/capi/doc/chfl_residue/copy.c}
/// @return A pointer to the new residue, or NULL in case of error.
///         You can use `chfl_last_error` to learn about the error.
CHFL_EXPORT CHFL_RESIDUE* chfl_residue_copy(const CHFL_RESIDUE* residue);

/// Get the number of atoms in a `residue` in the integer pointed to by `count`.
///
/// @example{tests/capi/doc/chfl_residue/atoms_count.c}
/// @return The operation status code. You can use `chfl_last_error` to learn
///         about the error if the status code is not `CHFL_SUCCESS`.
CHFL_EXPORT chfl_status chfl_residue_atoms_count(
    const CHFL_RESIDUE* residue, uint64_t* count
);

/// Get the list of atoms in the `residue` in the pre-allocated array `atoms`
/// of size `natoms`.
///
/// The `atoms` array size must be passed in the `natoms` parameter, and be
/// equal to the result of `chfl_residue_atoms_count`. The `atoms` array is
/// sorted.
///
/// @example{tests/capi/doc/chfl_residue/atoms.c}
/// @return The operation status code. You can use `chfl_last_error` to learn
///         about the error if the status code is not `CHFL_SUCCESS`.
CHFL_EXPORT chfl_status chfl_residue_atoms(
    const CHFL_RESIDUE* residue, uint64_t atoms[], uint64_t natoms
);

/// Get the identifier of a `residue` in the initial topology file in the
/// integer pointed to by `id`.
///
/// This function will return `CHFL_GENERIC_ERROR` if this residue does not
/// have an associated identifier.
///
/// @example{tests/capi/doc/chfl_residue/id.c}
/// @return The operation status code. You can use `chfl_last_error` to learn
///         about the error if the status code is not `CHFL_SUCCESS`.
CHFL_EXPORT chfl_status chfl_residue_id(
    const CHFL_RESIDUE* residue, uint64_t* id
);

/// Get the name of a `residue` in the string buffer `name`.
///
/// The buffer size must be passed in `buffsize`. This function will truncate
/// the residue name to fit in the buffer.
///
/// @example{tests/capi/doc/chfl_residue/name.c}
/// @return The operation status code. You can use `chfl_last_error` to learn
///         about the error if the status code is not `CHFL_SUCCESS`.
CHFL_EXPORT chfl_status chfl_residue_name(
    const CHFL_RESIDUE* residue, char* name, uint64_t buffsize
);

/// Add the atom at index `i` in the `residue`.
///
/// @example{tests/capi/doc/chfl_residue/add_atom.c}
/// @return The operation status code. You can use `chfl_last_error` to learn
///         about the error if the status code is not `CHFL_SUCCESS`.
CHFL_EXPORT chfl_status chfl_residue_add_atom(
    CHFL_RESIDUE* residue, uint64_t i
);

/// Check if the atom at index `i` is in the `residue`, and store the result in
/// `result`.
///
/// @example{tests/capi/doc/chfl_residue/contains.c}
/// @return The operation status code. You can use `chfl_last_error` to learn
///         about the error if the status code is not `CHFL_SUCCESS`.
CHFL_EXPORT chfl_status chfl_residue_contains(
    const CHFL_RESIDUE* residue, uint64_t i, bool* result
);

/// Get the number of properties associated with this `residue` in `count`.
///
/// @example{tests/capi/doc/chfl_residue/properties_count.c}
/// @return The operation status code. You can use `chfl_last_error` to learn
///         about the error if the status code is not `CHFL_SUCCESS`.
CHFL_EXPORT chfl_status chfl_residue_properties_count(
    const CHFL_RESIDUE* residue, uint64_t* count
);

/// Get the names of all properties of this `residue` in the pre-allocated array
/// `names` of size `count`.
///
/// `names` size must be passed in the `count` parameter, and be equal to the
/// result of `chfl_residue_properties_count`.
///
/// The pointers in `names` are only valid until a new property is added to the
/// residue with `chfl_residue_set_property`.
///
/// @example{tests/capi/doc/chfl_residue/list_properties.c}
/// @return The operation status code. You can use `chfl_last_error` to learn
///         about the error if the status code is not `CHFL_SUCCESS`.
CHFL_EXPORT chfl_status chfl_residue_list_properties(
    const CHFL_RESIDUE* residue, const char* names[], uint64_t count
);

/// Add a new `property` with the given `name` to this `residue`.
///
/// If a property with the same name already exists, this function override the
/// existing property with the new one.
///
/// @example{tests/capi/doc/chfl_residue/property.c}
/// @return The operation status code. You can use `chfl_last_error` to learn
///         about the error if the status code is not `CHFL_SUCCESS`.
CHFL_EXPORT chfl_status chfl_residue_set_property(
    CHFL_RESIDUE* residue, const char* name, const CHFL_PROPERTY* property
);

/// Get a property with the given `name` in this `residue`.
///
/// This function returns `NULL` if no property exists with the given name.
///
/// The user of this function is responsible to deallocate memory using the
/// `chfl_property_free` function.
///
/// @example{tests/capi/doc/chfl_residue/property.c}
/// @return A pointer to the property, or NULL in case of error.
///         You can use `chfl_last_error` to learn about the error.
CHFL_EXPORT CHFL_PROPERTY* chfl_residue_get_property(
    const CHFL_RESIDUE* residue, const char* name
);

/// Free the memory associated with a `residue`.
///
/// @example{tests/capi/doc/chfl_residue/chfl_residue.c}
/// @return `CHFL_SUCCESS`
CHFL_EXPORT chfl_status chfl_residue_free(const CHFL_RESIDUE* residue);

#ifdef __cplusplus
}
#endif

#endif
