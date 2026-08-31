#pragma once

/**
 * @class Repository
 * @brief Initializes a new version control repository.
 *
 * Creates the .gity directory structure,
 * initializes HEAD,
 * and creates an empty staging index.
 */


class Repository{
    public:
        /**
         * Creates a new repository in the current directory.
         */
        void init();
};