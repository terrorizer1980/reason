[@reason.version 3.8];

/* Copyright (c) 2015-present, Facebook, Inc. All rights reserved. */

type tt<'a> = int;

module LocalModule = {
  type accessedThroughModule =
    | AccessedThroughModule;
  type accessedThroughModuleWithArg =
    | AccessedThroughModuleWith(int)
    | AccessedThroughModuleWithTwo(int, int);
};

type notTupleVariant =
  | NotActuallyATuple(int, int);
type attr1 = ..;
type attr1 +=
  | A(int);
type attr1 +=
  | Point(int, int);
type attr1 +=
  | PointA({
      a: int,
      b: int,
    });

type notTupleVariantExtraParens =
  | NotActuallyATuple2(int, int);

type simpleTupleVariant =
  | SimpleActuallyATuple((int, int));

type tupleVariant =
  | ActuallyATuple((int, int));

let intTuple = (20, 20);

let notTupled: notTupleVariant =
  NotActuallyATuple(10, 10);

/* Doesn't work because we've correctly annotated parse tree nodes with explicit_arity! */
/* let notTupled: notTupleVariant = NotActuallyATuple (10, 10); */
let funcOnNotActuallyATuple =
    (NotActuallyATuple(x, y)) =>
  x + y;

/* let funcOnNotActuallyATuple (NotActuallyATuple (x, y)) = x + y; */
/* let notTupled: notTupleVariant = NotActuallyATuple intTuple; /*Doesn't work! */ */
/* At least the above acts as proof that there *is* a distinction that is
   honored. */
let simpleTupled: simpleTupleVariant =
  SimpleActuallyATuple((10, 10));

let simpleTupled: simpleTupleVariant =
  SimpleActuallyATuple(intTuple);

/*Works! */
let NotActuallyATuple(x, y) =
  NotActuallyATuple(10, 20);

let yesTupled: tupleVariant =
  ActuallyATuple((10, 10));

let yesTupled: tupleVariant =
  ActuallyATuple(intTuple);

type threeForms =
  | FormOne(int)
  | FormTwo(int)
  | FormThree;

let doesntCareWhichForm = x =>
  switch (x) {
  | FormOne(q)
  | FormTwo(q) => 10
  | FormThree => 20
  };

let doesntCareWhichFormAs = x =>
  switch (x) {
  | FormOne(q) as _ppp
  | FormTwo(q) as _ppp => 10
  | FormThree => 20
  };
type otherThingInheritedFrom = [ | #Base];
type colorList1 = [
  otherThingInheritedFrom
  | #Red
  | #Black
];

type colorList2 = [
  | #Red
  | #Black
  | otherThingInheritedFrom
];
type foo = [ | #foo];
type colorList3 = [
  colorList2
  | foo
  | #Red
  | #Black
  | foo
];

type lessThanGreaterThan<'a> =
  [< | #Red | #Black | #Blue > #Red #Black] as 'a;

/**
 * In order to get this to typecheck you must admit the row variable. But it strangely
 * does not type check on 4.06 and earlier.
 *
 * Therefore, we are including this in the unit tests but not the type checked tests.

  type colorList<'a> =
    [< | #Red(int, int) &(int) | #Black&(int, int) &(int) | #Blue] as 'a;

*/
1 + doesntCareWhichForm(FormOne(10));

1 + doesntCareWhichForm(FormTwo(10));

1 + doesntCareWhichForm(FormThree);

/* Destructured matching at function definition */
let accessDeeply =
    (LocalModule.AccessedThroughModule) => 10;

let accessDeeplyWithArg =
    (
      LocalModule.AccessedThroughModuleWith(x) |
      LocalModule.AccessedThroughModuleWithTwo(
        _,
        x,
      ),
    ) => x;

/* Destructured matching *not* at function definition */
let accessDeeply = x =>
  switch (x) {
  | LocalModule.AccessedThroughModule => 10
  };

let accessDeeplyWithArg = x =>
  switch (x) {
  | LocalModule.AccessedThroughModuleWith(x) => 10
  | _ => 0
  };

/* In OCaml's syntax, to capture the wrapped data, you do:
 *
 *   let myFunc x = function | `Blah (p as retVal) -> retVal`
 *
 * In OCaml's syntax, to capture the entire pattern you do:
 *
 *   let myFunc x = function | `Blah p as retVal -> retVal`
 */
let accessDeeply = x =>
  switch (x) {
  | LocalModule.AccessedThroughModule as _ppp => 1
  };

let accessDeeplyWithArg = x =>
  switch (x) {
  | LocalModule.AccessedThroughModuleWith(
      x as retVal,
    ) =>
    retVal + 1
  | LocalModule.AccessedThroughModuleWithTwo(
      x as retVal1,
      y as retVal2,
    ) =>
    retVal1 + retVal2 + 1
  };

/* Just to show that by default `as` captures much less aggresively */
let rec accessDeeplyWithArgRecursive =
        (x, count) =>
  switch (x) {
  | LocalModule.AccessedThroughModuleWith(x) as entirePattern =>
    /* It captures the whole pattern */
    if (count > 0) {
      0;
    } else {
      accessDeeplyWithArgRecursive(
        entirePattern,
        count - 1,
      );
    }
  | LocalModule.AccessedThroughModuleWithTwo(
      x,
      y,
    ) as entirePattern =>
    /* It captures the whole pattern */
    if (count > 0) {
      0;
    } else {
      accessDeeplyWithArgRecursive(
        entirePattern,
        count - 1,
      );
    }
  };

accessDeeplyWithArgRecursive(
  LocalModule.AccessedThroughModuleWith(10),
  10,
);

type combination<'a> =
  | HeresTwoConstructorArguments(int, int);

/** But then how do we parse matches in function arguments? */
/* We must require parenthesis around construction matching in function args only*/
let howWouldWeMatchFunctionArgs =
    (HeresTwoConstructorArguments(x, y)) =>
  x + y;

/* How would we annotate said arg? */
let howWouldWeMatchFunctionArgs =
    (
      HeresTwoConstructorArguments(x, y):
        combination<'wat>,
    ) =>
  x + y;

let matchingTwoCurriedConstructorsInTuple = x =>
  switch (x) {
  | (
      HeresTwoConstructorArguments(x, y),
      HeresTwoConstructorArguments(a, b),
    ) =>
    x + y + a + b
  };

type twoCurriedConstructors =
  | TwoCombos(
      combination<int>,
      combination<int>,
    );

let matchingTwoCurriedConstructorInConstructor =
    x =>
  switch (x) {
  | TwoCombos(
      HeresTwoConstructorArguments(x, y),
      HeresTwoConstructorArguments(a, b),
    ) =>
    a + b + x + y
  };

type twoCurriedConstructorsPolyMorphic<'a> =
  | TwoCombos(
      combination<'a>,
      combination<'a>,
    );

/* Matching records */
type pointRecord = {
  x: int,
  y: int,
};

type alsoHasARecord =
  | Blah
  | AlsoHasARecord(int, int, pointRecord);

let result =
  switch (
    AlsoHasARecord(10, 10, {x: 10, y: 20})
  ) {
  | Blah => 1000
  | AlsoHasARecord(a, b, {x, y}) =>
    a + b + x + y
  };

let rec commentPolymorphicCases:
  'a.
  option<'a> => int
 =
  fun
  | Some(a) => 1
  /* Comment on one */
  | None => 0;
type numbers =
  | Zero
  | One(int)
  | Three(int, int, int);
let something = Zero;
let testSwitch =
  switch (something) {
  | Zero
  | One(_) => 10
  | Three(_, _, _) => 100
  };

let thisWillCompileButLetsSeeHowItFormats =
  fun
  | Zero
  | Three(_, _, _) => 10
  | One(_) => 20;

/* Comment on two */
/**
 * GADTs.
 */
type term<_> =
  | Int(int): term<int>
  | Add: term<(int, int) => int>
  | App(term<'b => 'a>, term<'b>): term<'a>;

let rec eval: type a. term<a> => a =
  fun
  | Int(n) => n
  /* a = int */
  | Add => ((x, y) => x + y)
  /* a = int => int => int */
  | App(f, x) => eval(f, eval(x));

let rec eval: type a. term<a> => a =
  x =>
    switch (x) {
    | Int(n) => n
    /* a = int */
    | Add => ((x, y) => x + y)
    /* a = int => int => int */
    | App(f, x) => eval(f, eval(x))
    };

/* eval called at types (b=>a) and b for fresh b */
let evalArg = App(App(Add, Int(1)), Int(1));

let two =
  eval(App(App(Add, Int(1)), Int(1)));

type someVariant =
  | Purple(int)
  | Yellow(int);

let Purple(x) | Yellow(x) =
  switch (Yellow(100), Purple(101)) {
  | (Yellow(y), Purple(p)) => Yellow(p + y)
  | (Purple(p), Yellow(y)) => Purple(y + p)
  | (Purple(p), Purple(y)) => Yellow(y + p)
  | (Yellow(p), Yellow(y)) => Purple(y + p)
  };

type tuples =
  | Zero
  | One(int)
  | Two(int, int)
  | OneTuple(int, int);

let myTuple = OneTuple(20, 30);

let res =
  switch (myTuple) {
  | Two(y, z) =>
    try(Two(y, z)) {
    | Invalid_argument(_) => Zero
    }
  | One(_) =>
    switch (One(4)) {
    | One(_) => Zero
    | _ => Zero
    }
  | _ => Zero
  };

/* FIXME type somePolyVariant = [ `Purple int | `Yellow int]; */

let ylw = #Yellow((100, 100));

let prp = #Purple((101, 100));

let res =
  switch (ylw, prp) {
  | (#Yellow(y, y2), #Purple(p, p2)) =>
    #Yellow((p + y, 0))
  | (#Purple(p, p2), #Yellow(y, y2)) =>
    #Purple((y + p, 0))
  | (#Purple(p, p2), #Purple(y, y2)) =>
    #Yellow((y + p, 0))
  | (#Yellow(p, p2), #Yellow(y, y2)) =>
    #Purple((y + p, 0))
  };

let ylw = #Yellow(100);

let prp = #Purple(101);

let res =
  switch (ylw, prp) {
  | (#Yellow(y), #Purple(p)) => #Yellow(p + y)
  | (#Purple(p), #Yellow(y)) => #Purple(y + p)
  | (#Purple(p), #Purple(y)) => #Yellow(y + p)
  | (#Yellow(p), #Yellow(y)) => #Purple(y + p)
  };

/*
 * Now try polymorphic variants with *actual* tuples.
 * You'll notice that these become indistinguishable from multiple constructor
 * args! explicit_arity doesn't work on polymorphic variants!
 *
 * Way to resolve this (should also work for non-polymorphic variants):
 *
 * If you see *one* simple expr list that is a tuple, generate:
 *    Pexp_tuple (Pexp_tuple ..))
 *
 * If you see *one* simple expr list that is *not* a tuple, generate:
 *    Pexp..
 *
 * If you see *multiple* simple exprs, generate:
 *    Pexp_tuple..
 *
 * Though, I'm not sure this will even work.
 */
let ylw = #Yellow((100, 100));

let prp = #Purple((101, 101));

let res =
  switch (ylw, prp) {
  | (#Yellow(y, y2), #Purple(p, p2)) =>
    #Yellow((p + y, 0))
  | (#Purple(p, p2), #Yellow(y, y2)) =>
    #Purple((y + p, 0))
  | (#Purple(p, p2), #Purple(y, y2)) =>
    #Yellow((y + p, 0))
  | (#Yellow(p, p2), #Yellow(y, y2)) =>
    #Purple((y + p, 0))
  };

type contentHolder<'a> =
  | ContentHolderWithReallyLongNameCauseBreak(
      'a,
    )
  | ContentHolder('a);

/*
 * When pretty printed, this appears to be multi-argument constructors.
 */
let prp = #Purple((101, 101));

let res =
  switch (prp) {
  | #Yellow(y, y2) => #Yellow((y2 + y, 0))
  | #Purple(p, p2) => #Purple((p2 + p, 0))
  };

/*
 * Testing extensible variants
 */
type attr = ..;

/* `of` is optional */
type attr +=
  | StrString(string);

type attr +=
  | Point2(int, int);

type attr +=
  | Float(float)
  | Char(char);

type tag<'props> = ..;

type titleProps = {title: string};

type tag<'props> +=
  | Title: tag<titleProps>
  | Count(int): tag<int>;

module Graph = {
  type node = ..;
  type node +=
    | Str;
};

type Graph.node +=
  | Str = Graph.Str;

type water = ..;

type water +=
  pri
  | Ocean;

type water +=
  pri
  | MineralWater
  | SpringWater
  | TapWater
  | TableWater;
module Expr = {
  type Graph.node +=
    | Node
    | Atom;
};
module F = {
  type Graph.node +=
    pri
    | Node = Expr.Node;
};

type Graph.node +=
  pri
  | Node = Expr.Node
  | Atom = Expr.Atom;

type singleUnit =
  | MyConstructorWithSingleUnitArg(unit);
/* without single unit arg sugar */
MyConstructorWithSingleUnitArg();
/* with single unit arg sugar */
MyConstructorWithSingleUnitArg();
/* without single unit arg sugar */
#polyVariantWithSingleUnitArg();
/* with single unit arg sugar */
#polyVariantWithSingleUnitArg();

let x = #Poly;

/* Format doc attrs consistent: https://github.com/facebook/reason/issues/2187 */
type t =
  | /** This is some documentation that might be fairly long and grant a line break */
    A
  | /** Shorter docs */
    B
  | /** Some more longer docs over here that make sense to break lines on too */
    C;

/* https://github.com/facebook/reason/issues/1828 */
type widget_state = [
  | #DEFAULT /* here */
  | #HOVER
  | #ACTIVE
];
