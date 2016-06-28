#ifndef __RUIN_ASSERT_
#define __RUIN_ASSERT_

namespace Ruin 
{
    /**
     * Result Type enum
     */
    enum reResultType
    {
        rrt_Success,
        rrt_Fail
    };

    /**
     * Result struct
     * Structure returned by various functions to indicate success or failure.
     * Message field can be used to find an error string.
     */
    struct rsResult
    {
        reResultType result;
        const char* message;
    };

    /**
     * Result function
     * Creates a result with the given type and message.
     */
    inline rsResult rResult(
        reResultType result = rrt_Success,
        const char* message = "")
    {
        return rsResult { result, message };
    }

    /**
     *
     */
    inline bool rSuccessfulResult(rsResult result)
    {
        return result.result == rrt_Success;
    }

    /**
     * Assert function
     * Asserts that the given result is a success. If not halts the program.
     */
    void rAssert(rsResult);

    /**
     * Assert function
     * Asserts the given value is true.
     */
    void rAssert(bool);

    /**
     * Assert function
     * Asserts the given pointer is not null.
     */
    void rAssert(void*);
}

#endif