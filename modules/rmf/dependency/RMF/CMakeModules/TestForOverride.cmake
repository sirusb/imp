set(RMF_HAS_OVERRIDE CACHE BOOL "Whether the compiler supports the override keyword")

INCLUDE (CheckCXXSourceCompiles)

CHECK_CXX_SOURCE_COMPILES("struct Base {
    virtual void some_func(float);
};

struct Derived : Base {
    virtual void some_func(float) override;
};

        int main()
        {
            int a=6;
            return 0;
        }
"
 RMF_HAS_OVERRIDE)

message(STATUS "RMF override " ${RMF_HAS_OVERRIDE})

if(${RMF_HAS_OVERRIDE} MATCHES 1)
else()
set(RMF_HAS_OVERRIDE 0)
endif()

message(STATUS "RMF override " ${RMF_HAS_OVERRIDE})
