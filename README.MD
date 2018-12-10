# DepInject: A C++ Dependency Injection Framework

## Introduction
**DepInject** implements a [dependency injection](https://stackoverflow.com/questions/130794/what-is-dependency-injection)
framework in C++.
The entire implementation is contained in `depinject.h`, which must be #included in any compilation unit
referencing the framework.
Its author is Frederick Noon, <frs.noon@gmail.com>, and is publicly available at https://github.com/fnoon/DepInject.

Dependency injection presumes an overall architecture consisting of (a) application setup code, handling
program entry, configuration, and exit; and (b) application “business logic” containing the bulk of the runtime
functionality.
Classes within the business end of the program have dependencies on other classes to help perform their work:
these dependencies should be in the form of *interfaces,* pointers or references to abstract classes,
rather than containing objects declared as concrete classes wherever unit test partitioning is to be implemented.
In this way the actual class implementations they use can by *injected* into them at runtime,
rather than fixed within the class at compile time.
One can then change the setup code to “wire” the business classes together in different ways,
having them depend on production classes for production builds, or stubs and mocks for unit testing.

## Philosophy
There tend to be two approaches to dependency injection implementation: a *pull* model and a *push* model.
DepInject implements a pull model.
By this mechanism code requiring the services of a particular class retrieve a handle to one of its objects
from a call to some factory class interface.
By contrast, a push mechanism constructs objects for such code and then supply them either as parameters in its
constructor or has subsequent setter routines.

Some pros and cons of the two approaches:

* Pull model code has to call a particular factory class singleton, whereas push model code has its dependencies
“fall from the sky,” providing an illusion of generality.  Yet in both cases the dependent class must be explicitly coded to accept interfaces provided to it at runtime.

* Push model framework implementations tend to more complex.  However, this complexity is hidden from all but the program setup code.

* A pull model framework only needs to know how to construct concrete classes based on corresponding interface classes.
A push model framework also needs to be configured to know the dependencies of each class it is allocating so that they may be “wired” together, in order.

* Code using a pull model can request an already-configured factory object in an *ad hoc* manner, with no additional setup code.  In a push model, however, the dependent class’ interface must be modified to accept the new dependency, typically by changing the constructors.  Therefore all setup code, both for the production program and all test cases using the class, must be modified to use the new interface.  This can make throwing-in a log line in an arbitrary stretch of code more painful than it probably should be, IMHO.

## Basic Usage
DepInject clients interact with its `DepInject::Factory<>` class template.
One must first *declare* one or more interface classes to serve as dependencies,
together with routines to construct concrete objects derived from them as needed.
This is intended to be used by the application setup code as part of its program initialization.
Thereafter, classes can call the `Factory<>` to retrieve pointers to whatever dependencies
have been declared.

Exceptions are thrown when “bad things” happen, such as attempted instance retrievals preceding declarations or
object allocation failure.
RTTI is not used.

### Interface Declaration
Consider an example light bulb’s `IBulb` interface class, from which a concrete `Bulb` class is derived.
The setup code declares the `IBulb` class, using the function `Factory<IBulb>::declare()` which
is passed a routine that returns a pointer to a class derived from `IBulb`.
For example,

```c++
IBulb* AllocateBulb() {
    return new Bulb;
}

DepInject::Factory<IBulb>::declare(AllocateBulb);
```

A lambda can be used in lieu of a standalone allocation function:

```c++
DepInject::Factory<IBulb>::declare([]() -> IBulb* {return new Bulb;});
```

Indeed, there is a helper function that wraps this lambda style of simple allocator:

```c++
DepInject::basic_declaration<IBulb, Bulb>();
```

### Interface Retrieval
With the above `IBulb` declaration in place, a concrete `Lamp` class can retrieve a pointer
to a new `IBulb` by calling:

```c++
IBulb* bulb = DepInject::Factory<IBulb>::get()
```

Note that the lifetime of an object retrieved in such a manner is controlled by `DepInject::Factory<>`:
the user of the object should therefore never `delete` it!
This is because, by default, all calls to `get()` for objects of a given class **return the same object.**
Therefore a caller should probably try to store the returned pointer in a reference variable:

```c++
IBulb& bulb = *DepInject::Factory<IBulb>::get()
```

## Advanced Usage

### Tagged Declarations
In the *Basic Usage* section all objects of a given interface type were of the same concrete class,
as `get()` always used the same declared construction routine.
But one may occasionally need classes in ones code which rely on dependencies constructed in
ways different from the default — say, using a different concrete class or with different constructor arguments.
Including a *tag* in an interface declaration, and subsequent retrieval, one can accommodate
such classes.
The usage is straightforward; if in the setup code one writes:

```c++
IBulb* AllocateGaudyBulb() {
    return new GaudyBulb;
}

struct GaudyTag { };  // should be in an application header file

DepInject::Factory<IBulb, GaudyTag>::declare(AllocateGaudyBulb);
```

one can then use the alternative implementation so:

```c++
IBulb& bulb = *DepInject::Factory<IBulb, GaudyTag>::get();
```

### Unique Object Declarations
Following the above “basic usage,” DepInject constructs an object once, when first asked for, then keeps serving it to
however many clients request an instance of that interface.
If, however, clients require their own instances of the declared type it be declared with the alternative
`declare_unique()` call:

```c++
DepInject::Factory<IBulb>::declare_unique(AllocateBulb);
```

Thereafter, clients calling `Factory<IBulb>::get_unique()` will receive a pointer to their own, unique instance.
Such clients are then **responsible for the lifetime of the returned object.**
The client should ensure that the object is eventually destroyed, probably in its destructor (say, by wrapping the bare pointer as a `std::unique_ptr<>`),
although it is also possible to pass an allocator to `declare_unique()` that actually returns a pointer to a
static object, rather than one allocated on the heap.
The combination of interface type and tag must be unique among all `declare()` and `declare_unique()` calls,
so that code code using owned instances may be easier to reason about and track.


# References
This dependency injection framework suits my needs and preferences,
but I’ve found these other C++ implementations:

  https://github.com/admiyo/CppInject (described at https://adam.younglogic.com/2008/07/dependency-injection-in-c/)

  https://github.com/leapmotion/autowiring

  https://gpfault.net/posts/dependency-injection-cpp.txt.html

These are generally fancier than my design, with an obvious increase in complexity.
There are also differences in how these frameworks are used,
occasionally with some subtle limitations that mitigate much of the whiz-bang often displayed in them.
(For example, the requirement that all a class’s dependencies be passed in its constructor.)
But if one feels the need for, say, automatic dependency wiring, one can investigate them.

The included test / example code in the DepInject project uses the **[doctest](https://github.com/onqtam/doctest)**
C++ unit test framework.  I find it spiffy keen!

# License

I’m putting this project under the GNU Lesser General Public License, Version 3.
Let me know if you require a different license for your needs.
