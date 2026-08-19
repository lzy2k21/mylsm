set_project("my-lsm")
set_languages("c++20")

add_rules("mode.debug", "mode.release")
add_requires("gtest")

-- 静态库目标
target("skiplist")
    set_kind("static")
    add_files("src/skiplist/*.cpp")
    add_includedirs("include/skiplist", {public = true}) --public = true 让其他依赖skiplist头文件的可以自动继承该路径

-- 测试对象
target("test_skiplist")
    set_kind("binary")
    set_group("test")
    add_files("tests/test_skiplist.cpp")
    add_deps("skiplist")
    add_packages("gtest")