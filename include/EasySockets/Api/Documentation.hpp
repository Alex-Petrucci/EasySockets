#pragma once

/*
 * This file is for defining the documentation macros that will be used
 * for the library to ensure that it remains consistent across all platforms.
 *
 * Things which only need documentation in one place should not use this.
 *
 * Relevant files will be parsed by generate_docs.py, which will clear all the
 * existing docs and replace them with the ones defined in the file.
 */

/**
 * Marks where to generate documentation from generate_docs.py.
 * @param id the id that will be matched to get the documentation.
 */
#define ES_API_DOC(id)