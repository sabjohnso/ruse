ruse
====

## Objective

A meta-programming library enabling the low-cost development of
low-latency and high performance code and simple integration with
various Lisp dialects, primarily [Racket](https://racket-lang.org)


## Notions

#### Protocols

#### Monoids

##### Additive monoids

##### Multiplicative monoids

#### Functors

```c++
fmap(sqr, list(1, 2, 3)) // => list(1, 4, 9)
```

```c++
letf(list(1, 2, 3), [](auto x){ return
      x+x;
});
// => list(2, 4, 6)
```

#### Applicative functors
```c++
fapply(fmap(add, list(1, 2)), list(3, 4)); // => list(4, 5, 5, 6)
```

#### Monads
```c++
flatmap(list, list(1, 2, 3)); // => list(1, 2, 3)
```

```c++
flatmap(pure, list(1, 2, 3)); // => list(1, 2, 3)
```

```c++
letm(list(1, 2), [ ](auto x){ return
letm(list(3, 4), [](auto x){ return
      pure(x + y); }); });
// => list(4, 5, 5, 6)
```

```c++
letm(type<int>,    [ ](auto x){ return
letm(type<double>, [=](auto y){ return
      pure(x+y); }); });
// => type<double>
```

```c++
flatten(list(1, 2), list(3, 4)); // => list(1, 2, 3, 4)
```

#### Comonads

### Type proxies

Type proxies are a way of representing types as values. In this way,
they act as proxies for values. The variable template `type` is used
to in this respect. The following is a proxy for the type `int`:

```c++
type<int>;
```
Type proxies can be used to instantiate a type:
```c++
type<std::array<int, 2>>(1, 2);
```

The type of type proxies are specialization of the template `Type`:

```c++
type<int> == Type<int>{}; // => true
```

The variable template `type` strips `const`, `volatile` and reference
attributes from its template parameter;

```c++
type<const double&> == type<double>; // => true
```
Further, the template `Type` cannot be specialized with reference,
const or volatile qualified types.

Type proxies are monads. The unit constructor for the type proxy monad
is the function `type_of`:

```c++
type_of(3) == type<int>; // true
```
In a way it is a canonical example of a monad
be the value place in the `Type` context really cannot be retrieved
from the monad because it does not exist.  However, we are still able
to operate on it.  For example,

```C++
fmap([](auto x){ return std::to_string(x); }, type_of(int)); // => type<std::string>
```

This is very useful when chaining together functions that convert
values to type proxies:

```c++
constexpr auto value_type_of = []<class T>([[maybe_unused]] const T& x){
  return type<typename T::value_type>;,
};

flatmap(value_type_of, type_of(std::vector{1, 2, 3})); // => type<int>
```

The `type_from` using template is used to raise type proxies back up
to types.

```c++
std::same_as<int, type_from<type<int>>>; // => true
```


### Template proxies

In addition to types, sometimes it is useful to use proxies for
templates. A template proxy can be constructed using the `template_of` function

```c++
template_of(std::vector{1, 2, 3}) == template_<std::vector>; // => true
```

Whereas type proxies could be used to instantiate instances of the
type, template proxies can be used to instantiate specializations of
the template (proxies to them anyway):

```c++
template<std::vector>(type<int>); // type<std::vector<int>
```



### Value wrappers
Value wrappers are simple wrappers around values that propvide
additional information through the type, but main the same size and
layout as the value they are wrapping. Value wrappers must publish a
member type  named `value_type` and have a data member name `value`
with the type `value_type`, both publicly accessible.

Value wrappers are trivial monads and hence support the monad and
comonad interfaces.

### Tagged values
Tagged values are value wrappers with additional information to
provide a name.  The name is provided via a member type named
`name_type`, which must refer to an empty type.

There is a user literal (`_tag`) defined to make it easier to create
tagged values. The litteral creates a tag object that is applied to a
values through the function call operator.

```c++
"some-name"_tag(3.0);
```

### Pairs
The type template `pair` is similar to `std::pair` and is the fundamental
structual element used in **Ruse**.  A pair is created primarily with
the function `cons`:

```c++
cons(3.0, 'x')
```

#### Lists

##### Nothing

Lists are built out of nested pairs and the value `nothing`, which is
used to delimit the end of a list. The value `nothing` is a list

```c++
is_list(nothing); // => true
```

The value `nothing` is an empty list.

```c++
is_empty_list(nothing); // => true
```

There is a concept for empty lists.
```c++
EmptyList<decltype(nothing)>; // => true
```

##### Nonempty lists

Nested pairs terminated with `nothing` are nonempty lists.

```c++
is_list(cons(1, cons(2, nothing))) // => true
```

Lists are more convieniently constructed using the `list` function;

```c++
list(1, 2) == cons(1, cons(2, nothing)) // => true
```

Lists can be constructed using nonuniform element types, including
nested lists
```c++
list('a', 3, 2.4, "Hello, World!",
     list(1, 2, 3));
```
which gives **Ruse** the feeling of a dynamically typed lange.
However, it is not. And there are some limitations. To access a
particular element of the list by index needs the indexing to be done
by type, rather than value:

```c++
list_ref(nat<1>, list('x', 2.3, "cat food")); // => 2.3
```

##### Homogeneous Lists

Some lists can have elements accessed by an index value.  Theey are
homogeneous lists or hlists.  Having a homogeneous type means that the
return type can be determined regardless of the value.  Accessing
elments of an hlist is done in a similar manner as other lists:

```c++
hlist_ref(1, list(1, 2, 3)) // => 2
```

If the index is out of bounds, it is an error and an exception will be
thrown.

##### Strings

##### Property Lists

```c++
plist_ref("y"_tag, list("x"_tag(1), "y"_tag('y'), "z"_tag(3))); // => "z"_tag(3)
```
The function `plist_ref_` is similar toe `plist_ref` but it removes
the tag from the result:

```c++
plist_ref_("y"_tag, list("x"_tag(1), "y"_tag(2), "z"_tag(3))); // => 2
```



#### Vectors

Like lists, vectors are nested pairs.  The primary difference between
vectors and lists is that the lists nest the second element while
vectors nest the first. The results in lists being constructed by
pushing a value onto the front of an existing list and vectors being
constructed by pushing a value onto the back of an existing vector.

Vectors were implemented as an improvement over lists with regard to
the similarity to c++ datastructures defined with `struct` or `class`.
Those improvements are with regard to the layout of the elements which
is important for some aspects of **Ruse** records but is not
significant otherwise.

# Empty vectors

The value `empty_vector` is a vector

```c++
empty_vector
```

(a, (b, ( c, (d, nothing)))) - list
(((vector_head{a}, b), c), d)  - vector

Additionally, vectors do not


## Facilities

### Compile-time list processing

### Compile-time code transformation

### Alternative record type definitions
```c++
constexpr auto point = (record, "point"_name,
 (("x"_field, type<double>),
  ("y"_field, type<double>),
  ("z"_field, type<double>)));

constexpr auto p = make(point, 1.0, 2.0, 3.0);

static_assert(p == list("point"_tag(
                          list("x"_tag(1.0),
                               "y"_tag(2.0),
                               "z"_tag(3.0)))));

static_assert(lookup("x"_field) == 1.0);
static_assert(lookup("y"_field) == 2.0);
static_assert(lookup("z"_field) == 3.0);

```

### Language and grammar oriented programming

### Lisp style macro expansion
#### Syntax Transformers
#### Symbol Macros
#### Character Macros

### Reflection
#### Schema generation from types
#### Formatting and serialization
#### Documentation generation

### JSON and JSON Schema
#### JSExpr
```c++
obj(prop("triangle",
         arr(
           obj(prop("point",
                    obj(prop("x", 0.0),
                        prop("y", 0.0),
                        prop("z", 0.0)))),
           obj(prop("point",
                    obj(prop("x", 0.0),
                        prop("y", 1.0),
                        prop("z", 0.0)))),
           obj(prop("point",
                    obj(prop("x", 0.0),
                        prop("y", 0.0),
                        prop("z", 1.0)))))));
```

#### JSON Schema based concepts for flexible typing
```c++
constexpr static auto point_schema = json_schema<
  obj(prop("type", "object"),
      prop("properties",
         obj(prop("x", obj(prop("type", "number"))),
             prop("y", obj(prop("type", "number"))),
             prop("z", obj(prop("type", "number"))))),
      prop("required", arr("x", "y", "z"))
      prop("additionalProperties", false))>;

template<typename T>
concept Point = JSONSchema<T, point_schema>;
```

### Work
 - [x] Core functionality
    - [x] Type proxies
    - [x] Tags
    - [x] Lists
       - [x] Simple lists
       - [x] Homogeneous lists
       - [x] Property lists
    - [x] Hoisted lists
    - [x] Protocols
       - [x] Functors
          - [x] Applicative functors
          - [x] Monads
       - [x] Monoids
    - [ ] Vectors
       - [x] Regular Vectors
          - [x] Tests
          - [x] Implementation
          - [x] Documentation
       - [x] Homogeneous Vectors
          - [x] Tests
          - [x] Implementation
          - [x] Documentation
       - [x] Property Vectors
          - [x] Tests
          - [x] Implementation
          - [x] Documentation
 - [ ] Code Generation
 - [ ] Compile-time execution
    - [ ] Modules
       - [ ] Provide
       - [ ] Require
 - [ ] JSON
    - [ ] JSExpr
    - [ ] JSON Schema
       - [ ] Validation
       - [ ] Schema concepts
 - [ ] Macro expansion
    - [ ] Syntax transformers
    - [ ] Symbol macros
    - [ ] Character macros
 - [ ] Racket interoperability
 - [ ] Records
    - [ ] Testing
    - [ ] Concepts
    - [ ] Implementation
    - [ ] Documentation
    - [ ] Migration to reference
 - [ ] Reader
    - [x] Testing
    - [x] Implementation
    - [x] Documentation
    - [x] Migration to reference
    - [-] Generic `ask`
      The added complexity is concidered to high for the benefit
    - [-] Generic `asks`
      The added complexity is concidered to high for the benefit
    - [-] Generic `local`
      The added complexity is concidered to high for the benefit
 - [ ] Writer
    - [ ] Testing
    - [ ] Implementation
    - [ ] Documentation
    - [ ] Migration to reference
 - [ ] Reflection
    - [ ] Enumerations
       - [ ] Type name
       - [ ] Enumerated Value Names
       - [ ] Iterating over enumerated values
       - [ ] Serialization and Formatting
    - [ ] Structs and Classes
       - [ ] Type name
       - [ ] Data Member Names
       - [ ] Data Member Types
       - [ ] Iterating over data members
       - [ ] Member function names
       - [ ] Member function signatures
       - [ ] Serialization and Formatting
 - [ ] Grammar
    - [ ] Syntax
       - [ ] BNF syntax
       - [ ] Affix syntax
       - [ ] Two Level syntax
    - [ ] Recognizers
    - [ ] Transducers
    - [ ]

 - [x] State
    - [x] Testing
    - [x] Implementation
    - [x] Documenation
    - [x] Migration to reference
    - [-] Generic `get`
      The added complexity is concidered to high for the benefit
    - [-] Generic `select`
      The added complexity is concidered to high for the benefit
    - [-] Generic `put`
      The added complexity is concidered to high for the benefit
    - [-] Generic `modify`
      The added complexity is concidered to high for the benefit
    - [-] Generic `run_state`
      The added complexity is concidered to high for the benefit
 - [ ] Contextual
    - [ ] Testing
    - [ ] Implementation
    - [ ] Documentation
    - [ ] Comparison with existing protocols for lists, type, state, and reader
    - [ ] Migration to reference if comparison is satisfactory
