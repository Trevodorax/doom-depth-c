#ifndef DOOM_DEPTH_C_JSON_H
#define DOOM_DEPTH_C_JSON_H

typedef struct Json Json;
struct Json {
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
    size_t nb_elements;
    char **keys;
    Json *values;
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
Json *get_json_from_file(const char *file_path);

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
Json *parse_json(char **json_string);

/**
 * @brief recursively frees a Json completely
 *
 * @sideeffects frees the memory of the complete json at all levels of nesting
 * @param json the json to free
 * @author Paul Gaudeaux
 * @date 17/09/23
 */
void free_json(Json *json);

#endif //DOOM_DEPTH_C_JSON_H