# Replace 'BUILD_TEST ON' with 'BUILD_TEST OFF'
#
# This is used as a patch step after cloning shapelib with FetchContent because we don't want to
# build the tests, and it doesn't provide a proper way to disable them.
file(READ ${FILE} FILE_CONTENTS)
string(
    REPLACE "BUILD_TEST ON"
            "BUILD_TEST OFF"
            FILE_CONTENTS
            ${FILE_CONTENTS}
)
file(WRITE ${FILE} ${FILE_CONTENTS})
