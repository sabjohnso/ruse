ruse
====

```c++
namespace MossUDP {

  RUSE_ENUM_CLASS(
    packet_type,
    char,
    ((data_packet, 'U', "a data packet (carries 0 or more data messages"_doc),
     (heartbeat, 'H', "heartbeat"_doc),
     (end_of_session, 'E', "end of session"_doc)));

  constexpr auto moss_packet = record{
    .name = "moss_packet"_,
    .attributes =
      list(block_size = "packet_length"_, dispatch_key = "packet_length"_),
    .slots = list(
      slot{.name = "packet_length"_, type = num<4>},
      slot{.name = "session"_,       type = alnum<10>},
      slot{.name = "sequence"_,      type = num<4>},
      slot{.name = "packet_type"_,   type = type<packet_type>})};

  constexpr auto moss_block = record{
    .name = "moss_block"_,
    .attributes = list(payload_size = "message_length"_)
    .slots = list(slot{.name = "message_length"_, .type = num<2>})};

  constexpr auto moss_heartbeat = record{
    .name = "message"_,
    .slots = list(slot{.name = "next_sequence_number", .type = num<4>})};

  // moss is a dispatching sequence block header

  constexpr auto moss_parser = lambda("payload-parser"_)(
    dispatch(moss_packet)
    | cons(packet_type::data_packet,    block_sequence(moss_block) >> "payload_parser"_ ),
    | cons(packet_type::heartbeat,      moss_heartbeat),
    | cons(packet_type::end_of_session, nothing));

} // namespace MossUDP
```

``` c++
list(lambda, list("x"_sym "y"_sym),
     list(add, "x"_sym, "y"_sym));
```

```scheme
(lambda (x y) (add x y))
```

``` c++

constexpr auto let = syntax_parse(
  "let"_sym,
  list(list(_, list(list("x"_sym[id], "mx"_sym[expr])), "body"_sym, __1),
       syntax_quote(
         list(list(lambda, list("x"_sym), "body"_sym, __) "mx"_sym))),
  list(list(_, list(list("x"_sym[id], "mx"_sym[expr]),
                    "more_bindings"_sym[expr], __) "body"_sym, __1),
       syntax_quote(
         list(lambda, list("x"_sym)
              list("let"_sym, list("more_bindings"_sym, __), "body"_sym, __)))));
```

``` c++
ˡ(let, ˡ(ˡ("x"_sym, 1),
         ˡ("y"_sym, 2)),
  ˡ(add, "x"_sym, "y"_sym))
```


```scheme

(syntax-parser let
 [(_ ([x:id e:expr]) body:expr ...+)
  #'((λ (x) body ...) e)]
 [(_ ([x:id e:expr] more-bindings:expr ...) body:expr ...+)
  #'((λ (x)
       (let (more-bindings ...)
         body ...)) e)])

```

```scheme
(let ([x 1]
      [y 2])
  (add x y))
```


```c++

syntax let = 
  syntax_parser(
    datum_literals(=),

  [let(x:id = e:expr)(body:expr),
   syntax_quote([&](auto const& x){ return body }(e))],

  [let(x:id = e:expr, more_bindings:expr ...)( body:expr)
   syntax_quote([&](auto const& x){ return let(more_bindings ...)(body);}(e))]);

```

```c++

syntax letM =
  syntax_rules(
    [=],
    [letM(x = me)(body),
     bindM(me, [&](const auto& x){ return body; })]

    [letM(x = me, more_bindings ...+)(body)
     bindM(me, [&](const auto& x){
       return letM(more_bindings ...)(body)})]);
```

```c++
letM(x = list(1, 2),
     y = list(3, 4))(
  returnM(x + y));
```

```c++

letM((("x"_, list(1, 2)),
      ("y"_, list(3, 4))),
     returnM("x"_ + "y"_));

lambda(("x"_, "y"_) "x"_ + "y"_)

constexpr auto addM = eval(
  lambda(
    ("xs"_, "ys"_),
    letM(("x"_ = "xs"_,
          "y"_ = "ys"_),
         returnM("x"_ + "y"_))));
```

```c++
// definition of a module that provides a function sqr
constexpr auto squaring_module =  module(
  ruse, // ruse is the module language: the language used to expand

  type(list(a),
       cons(a, list(a)),
       nil),
  
  let(make_list() = nil),
  let(make_list(x, xs ...) = cons(x, make_list(xs ...))),

  let(rappend(nil, ys) = ys),
  let(rappend(cons(x, xs),  ys) = rappend(xs, cons(x, ys))),

  let(reverse(xs) = rappend(xs, nil)),

  let(append(xs, ys) = rappend(reverse(xs), ys)),

  let(flatmap(f, nil) = nil),
  let(flatmap(f, cons(x, xs)) = append(f(x), flatmap(f, xs))),
  
```

```scheme
(module functors ruse

  (provide functor)
  
  (protocol (functor [M : (-> type type)])
    (decl (fmap [a : type] [b : type])
      (-> (-> a b) (M a) (M b)))))


(module lists ruse
  (type (list 'a)
    (cons 'a (list 'a))
    nil)

  (let (list) nil)
  (let (list x xs ...) (cons x (list xs ...)))

  (let (rappend nil ys) ys)
  (let (rappend (cons x xs) ys) (rappend xs (cons x ys)))


  (let (reverse xs) (rappend xs nil))

  (let (append xs ys) (rappend (reverse xs) ys))


  (let (flatmap f nil) nil)
  (let (flatmap f (cons x xs))
    (append (f x) (flatmap f xs))))





```
