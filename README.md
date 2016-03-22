##Repository Purpose##

For comparitive designs of different approaches to intrument geometry for version 2. This repository specifically looks at fresh approaches to instrument representation. A major requirement is the ability to rapidly extract key detector information required for data reduction. See [requirements](https://github.com/mantidproject/documents/blob/master/Design/Instrument-2.0/requirements-v2.md) for more context.

This repository is designed to host several prototypes aiming for the same purposes.

##What we expect the prototypes to do##

A distilled version of the requirements that the prototypes should address:

* Changing some part of the instrument geometry should not result in a deep copy.
* Allow each component to have a "shape" where each the shape can be shared across possibly 100,000's pixels but 
  interrogating the location of the shape produces its "real" position
* Store key/value pairs (limited allowed types is fine) against a component. We need to handle cases where a parameter value is set across either a composite component or the whole instrument, e.g. He3 pressure in a tube. The lookup in this cases would still be through the leaf component so the client is unaware that it is shared but we don't create potentially a great many entries that are the same. The lookup for this still needs to be reasonably quick, e.g. accessing `efixed` for a given component in `Indirect` mode during `ConvertUnits`.
* Fast access to frequently-queried geometry quantities such as `l1`, `l2`, `theta`, `phi` (essentially what would be in a `.dat` file)

##Building the prototype##

CMake is required to configure the prototype.

[GoogleTest](https://github.com/google/googletest) is required to build the prototype, and must be downloaded and built separately:

```
$ git clone https://github.com/google/googletest.git
$ cd googletest
$ cmake .
$ make
```

There are two variables that may need to be set when configuring the prototype: `GTEST_SOURCE_DIR` and `GTEST_BUILD_DIR`.

`GTEST_SOURCE_DIR` should be the path to the googletest topmost directory.
`GTEST_BUILD_DIR` should be the path to the googletest build directory (will be the same as `GTEST_SOURCE_DIR` in case of in-source builds).

Both variables have defaults that assume googletest was built in-source (as shown above) and is adjacent to instrument-prototype. How instrument-prototype is built doesn't matter. For example:

```
- Dev
|-- googletest
|-- instrument-prototype
```

If this is not how your directories are set up, or if you built googletest as an out of source build, you will need to provide those variables to CMake when configuring the prototype. This can be done via the GUI or the commandline, e.g.:

```
$ git clone https://github.com/DMSC-Instrument-Data/instrument-prototype.git
$ cd instrument-prototype
$ mkdir build
$ cd build
$ cmake -DGTEST_SOURCE_DIR="../../googletest" -DGTEST_BUILD_DIR="../../googletest" ..
$ make
```

CMake will issue an error if the directories are incorrect (if it cannot find the include/library files required).

