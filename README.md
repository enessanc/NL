This is a basic header-only library for communicating over
network safely on top of TCP protocol with the usage of library
named asio standalone.

To use it as a library just add to your source/external directory within your module.

    add_subdirectory(...the directory of NL...)
    target_link_library({PROJECT_NAME} {VISIBILITY_CHOICE} NL)

That should be enough to use it. You can find the basic usage of these
headers in test directory.