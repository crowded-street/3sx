zpp serializer
==============
A single header only standard C++ serialization framework.

Abstract
--------
In C++ there is no standard way of taking an object as-is and transforming it into a language
independent representation, that is, serialize it.

Serialization frameworks are really common in C++, and they all come with difference promises and have their advantages and disadvantages.
I've had the pleasure to seek a serialization framework that would turn my classes into data
in the most effortless manner, without caring much about the format, and without doing unnecessary logic.

Some frameworks support many common formats such as json, xml, and such; Some frameworks provide
you with the highest level of performance using zero copy techniques, thus supporting only binary format;
Some frameworks require you to run a script that generates the C++ code that serializes your classes;

While there are many excellent serialization frameworks, the diversity of the features and complexity often
make them hard to adopt, some of them require you to change existing code to integrate them, or even set up
your build environment differently.

I finally reached the conclusion that I do not need all those features.
I definitly do not want to either pay unnecessary price in performance to serialize my classes into a textual format, change the code of
my already existing classes, modify my build systems, write my classes in another format and compile it into C++.

What I needed was to have my classes serialized in a zero overhead manner into binary, with the ability to serialize
objects by their dynamic type, allowing easy dispatch logic between a server and client side, with little to no
change to my already existing classes.

Motivation
----------
Provide a single, simple header file, that would enable one to:
* Enable save & load any STL container / string / utility into and from a binary form, in a zero overhead approach.
* Enable save & load any object, by adding only a few lines to any class, without breaking existing code.
* Enable save & load the dynamic type of any object, by a simple one-liner.

Contents
--------
* To enable save & load of any object, add the following lines to your class, `object_1, object_2, ...` being the non-static
data members of the class.
```cpp
    friend zpp::serializer::access;
    template <typename Archive, typename Self>
    static void serialize(Archive & archive, Self & self)
    {
        archive(self.object_1, self.object_2, ...);
    }
```
If your class does not have a default constructor, define one as private.
* To enable save & load of the dynamic type of any object, you have to register it, and have the base type derive from `zpp::serializer::polymorphic`.
Given the classes `v1::protocol::client_hello`, `v1::protocol::server_hello`, and `v1::protocol::sleep`, all derive from `protocol::command`.
```cpp
// protocol.h
class protocol::command : public zpp::serializer::polymorphic
{
public:
    virtual void operator()(protocol::context &) = 0;
    virtual ~command() = default;
};

// protocol.cpp
namespace
{
zpp::serializer::register_types<
   zpp::serializer::make_type<v1::protocol::client_hello, zpp::serializer::make_id("v1::protocol::client_hello")>,
   zpp::serializer::make_type<v1::protocol::server_hello, zpp::serializer::make_id("v1::protocol::server_hello")>,
   zpp::serializer::make_type<v1::protocol::sleep, zpp::serializer::make_id("v1::protocol::sleep")>,
   // ...
> _;
}
```
* Save and load objects into a vector of data, in this example we show polymorphic serialization which
has an overhead of 8 bytes serialization id, per polymorphic object being serialized.
```cpp
// The data of the objects we serialize, the vector will grow and shrink as we serialize
// data to/from the vector.
std::vector<unsigned char> data;

// Turns an object into data.
zpp::serializer::memory_output_archive out(data);

// Turns data into objects.
zpp::serializer::memory_input_archive in(data);

// Create a sleep command.
std::unique_ptr<protocol::command> command = std::make_unique<v1::protocol::sleep>(60s);

// Serialize a unique pointer of an object whose zpp::serializer::polymorphic is a base class,
// prepends 8 bytes of the serialization id, then the derived class is serialized.
out(command);

// ...
// Deserializes a unique pointer of an object whose zpp::serializer::polymorphic is a base class,
// loads 8 bytes of the serialization id, constructs a `v1::protocol::sleep` then deseializes into it.
in(command);

// Run the command, any command has its own logic.
(*command)(protocol_context);
```

* You can serialize multiple objects in the same line:
```cpp
out(object_1, object_2, ...);
in(object_1, object_2, ...);
```

* You can request serializtion without the polymorphic overhead, thus the static type
is serialized and only this type can be loaded in the other end.
```
out(*command);
in(*command);
```

* Serializing STL containers and strings, first stores a 4 byte size, then the elements:
```
std::vector<int> v = { 1, 2, 3, 4 };
out(v);
in(v);
```
The reason why the default size type is of 4 bytes (i.e `std::uint32_t`) is that most programs
almost never reach a case of a container being more than ~4 billion items, and it may be unjust to
pay the price of 8 bytes size by default.

* For specific size types that are not 4 bytes, use `zpp::serializer::size_is<SizeType>()`:
```
std::vector<int> v = { 1, 2, 3, 4 };
out(zpp::serializer::size_is<std::uint16_t>(v));
in(zpp::serializer::size_is<std::uint16_t>(v));
```
Make sure that the size type is large enough for the serialized object, otherwise less items
will be serialized, according to conversion rules of unsigned types. Uncareful use may lead to
erroneuos code.

* You may use `memory_view_input_archive`/`memory_view_output_archive` that receives a view type or pointer and size rather than
a vector which requires ownership and memory allocation. In contrary to the owning archives, the view types are not altered, and
you should use the `offset()` function to determine the position of the processed input and output.

* Serialization using argument dependent lookup is also possible:
```cpp
namespace my_namespace
{
struct adl
{
    int x;
    int y;
};

template <typename Archive>
void serialize(Archive & archive, adl & adl)
{
    archive(adl.x, adl.y);
}

template <typename Archive>
void serialize(Archive & archive, const adl & adl)
{
    archive(adl.x, adl.y);
}
}
```

* Objects that do not derive from `zpp::serializer::polymorphic` do not need registration
and have no overhead at all.

Example
-------
```cpp
#include "serializer.h"
#include <vector>
#include <iostream>

class point
{
public:
    point() = default;
    point(int x, int y) noexcept :
        m_x(x),
        m_y(y)
    {
    }

    friend zpp::serializer::access;
    template <typename Archive, typename Self>
    static void serialize(Archive & archive, Self & self)
    {
        archive(self.m_x, self.m_y);
    }

    int get_x() const noexcept
    {
        return m_x;
    }

    int get_y() const noexcept
    {
        return m_y;
    }

private:
    int m_x = 0;
    int m_y = 0;
};

class person : public zpp::serializer::polymorphic
{
public:
    person() = default;
    explicit person(std::string name) noexcept :
        m_name(std::move(name))
    {
    }

    friend zpp::serializer::access;
    template <typename Archive, typename Self>
    static void serialize(Archive & archive, Self & self)
    {
        archive(self.m_name);
    }

    const std::string & get_name() const noexcept
    {
        return m_name;
    }

    virtual void print() const
    {
        std::cout << "person: " << m_name;
    }

private:
    std::string m_name;
};

class student : public person
{
public:
    student() = default;
    student(std::string name, std::string university) noexcept :
        person(std::move(name)),
        m_university(std::move(university))
    {
    }

    friend zpp::serializer::access;
    template <typename Archive, typename Self>
    static void serialize(Archive & archive, Self & self)
    {
        person::serialize(archive, self);
        archive(self.m_university);
    }

    virtual void print() const
    {
        std::cout << "student: " << person::get_name() << ' ' << m_university << '\n';
    }

private:
    std::string m_university;
};

namespace
{
zpp::serializer::register_types<
   zpp::serializer::make_type<person, zpp::serializer::make_id("v1::person")>,
   zpp::serializer::make_type<student, zpp::serializer::make_id("v1::student")>
> _;
} // <anynymous namespace>

static void foo()
{
    std::vector<unsigned char> data;
    zpp::serializer::memory_input_archive in(data);
    zpp::serializer::memory_output_archive out(data);

    out(point(1337, 1338));

    point my_point;
    in(my_point);

    std::cout << my_point.get_x() << ' ' << my_point.get_y() << '\n';
}

static void bar()
{
    std::vector<unsigned char> data;
    zpp::serializer::memory_input_archive in(data);
    zpp::serializer::memory_output_archive out(data);

    std::unique_ptr<person> my_person = std::make_unique<student>("1337", "1337University");
    out(my_person);

    my_person = nullptr;
    in(my_person);

    my_person->print();
}

static void foobar()
{
    std::vector<unsigned char> data;
    zpp::serializer::memory_input_archive in(data);
    zpp::serializer::memory_output_archive out(data);

    out(zpp::serializer::as_polymorphic(student("1337", "1337University")));

    std::unique_ptr<person> my_person;
    in(my_person);

    my_person->print();
}
```

Freestanding Implementation
--------------------------
The library also supports experimental freestanding mode, to allow running in an environment
without exceptions and rtti.

To enable freestanding mode, define `ZPP_SERIALIZER_FREESTANDING` preprocessing macro.

In this mode polymorphic serialization is not supported, and error checking
is done via return values.

The returned error type is `zpp::serializer::freestanding::error`. The numeric value of the error is of
the values in the enum class `zpp::serializer::error` and is accessible by `code()` member function.
The error message is accessible by calling the `message()` member function, as a `std::string_view`.

In this mode serialization functions should be declared with `auto` as the return type, and return the result
from the `archive`, like so:
```cpp
    template <typename Archive, typename Self>
    static auto serialize(Archive & archive, Self & self)
    {
        return archive(self.m_x, self.m_y);
    }
```

Error checking is done like so:
```cpp
    std::vector<unsigned char> data;
    zpp::serializer::memory_input_archive in(data);
    zpp::serializer::memory_output_archive out(data);

    if (auto result = out(point(1337, 1338)); !result) {
        std::cout << "Error: " << result.code() << " message: " << result.message() << '\n';
        // return failure / throw
    }

    point my_point;
    if (auto result = in(my_point); !result) {
        std::cout << "Error: " << result.code() << " message: " << result.message() << '\n';
        // return failure / throw
    }

    std::cout << my_point.get_x() << ' ' << my_point.get_y() << '\n';
```

A Python Version
----------------
A compact python version of the library can be found here: https://github.com/eyalz800/zpp_serializer_py.
You can use this library to intercommunicate with this one. The python version does not support variant/optional.

Requirements
------------
This framework requires a fully compliant C++14 compiler, including RTTI and exceptions enabled.
One can easily overcome the RTTI requirement by using the following project: https://github.com/eyalz800/type_info.
Disclaimer: registering polymorphic types can be slower in C++14 compared to C++17 due to the use of `shared_timed_mutex` instead of `shared_mutex`.
