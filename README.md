ruse
====

## Objective

A meta-programming library enabling the low-cost development of
low-latency and high performance code and simple integration with
various Lisp dialects, primarily [Racket](https://racket-lang.org)

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
