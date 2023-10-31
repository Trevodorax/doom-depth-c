#ifndef DOOM_DEPTH_C_JSON_H
#define DOOM_DEPTH_C_JSON_H

typedef struct json_t json_t;
struct json_t {
    /**
     * a: array
     * o: object
     * s: string
     * n: number
     */
    char type;

    // if number
    int number;

    // if string
    char *string;

    // if array or object
    int nb_elements;
    char ** keys;
    json_t * values;
};

/**
 * @brief Parses json file and stores it in a Json struct
 *
 * @param file_path Path of the file to parse. Should be a well formatted .json file. (doesn't take ownership)
 * @return The parsed Json object (gives ownership)
 * @errors Returns NULL in the following cases:
 * - impossible to open file
 * - bad .json format in file
 * - cannot allocate memory
 * @author Paul Gaudeaux
 * @date 26/09/23
 */
json_t * get_json_from_file(const char *file_path);

/**
 * @brief Parses a string and returns a Json struct
 *
 * @return The parsed Json object (gives ownership)
 * @errors Returns NULL in the following cases:
 * - bad .json format
 * - cannot allocate memory
 * @sideeffects moves the pointer to string to the end of the parsed json
 * @param json_string
 * @author Paul Gaudeaux
 * @date 26/09/23
 */
json_t * parse_json(char ** json_string);

/**
 * @brief Returns the index of the element with given key
 *
 * @param json Searched json of type 'o'
 * @param key Key to find index of
 * @error If there is no object with given key, returns -1
 * @return The index of the object with the given key (or -1)
 */
int get_index_of_key(json_t * json, char * key);

/**
 * @brief Retrieves a Json object for a given key within a parent Json object.
 *
 * @param json Pointer to the parent Json object of type 'o'.
 * @param key  Key to be searched within the parent Json object.
 * @return Pointer to the Json object corresponding to the key, or NULL if not found.
 */
json_t * get_object_at_key(json_t * json, char * key);

/**
 * @brief recursively frees a Json completely
 *
 * @sideeffects frees the memory of the complete json at all levels of nesting
 * @param json the json to free
 * @author Paul Gaudeaux
 * @date 17/09/23
 */
void free_json(json_t * json);

/**
 * @brief Writes a json to a file
 *
 * @param json The json to write
 * @param file_path The file to write it in
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int write_json_to_file(json_t * json, char * file_path);

/**
 * @brief Adds an entry to a json object
 *
 * @param object The object to add the values in
 * @param key
 * @param value
 */
void add_key_value_to_object(json_t **object, const char * key, json_t * value);

#endif //DOOM_DEPTH_C_JSON_H
