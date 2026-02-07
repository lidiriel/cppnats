# CppNats

A C++20 client library for [NATS](https://nats.io/), built on top of the official [cnats](https://github.com/nats-io/nats.c) C client.

Development in progress...

## Requirements

- CMake 3.30+
- C++20 compatible compiler
- A running NATS server (for testing)

## Building

```bash
cmake -B build
cmake --build build
```

## Usage

### Simple connection

```cpp
#include "cppnats.hpp"

CppNats::Client client;
client.connect("nats://localhost:4222");
// ...
client.close();
```

### Connection with options

```cpp
#include "cppnats.hpp"

CppNats::Options opts;
opts.addServer("nats://localhost:4222");
opts.timeout(5000);
opts.userInfo("user", "password");

CppNats::Client client;
client.connect(opts);
// ...
client.close();
```

## Extracting `nats-server` from a Docker container

If you don't have `nats-server` installed locally but have the official NATS Docker image, you can copy the binary out of a running container.

1. Start a NATS container (if not already running):

```bash
docker run -d --name nats_server nats:latest
```

2. Find your container name:

```bash
docker ps --filter ancestor=nats:latest --format '{{.Names}}'
```

3. Copy the binary from the container to your local `build/bin/` directory:

```bash
docker cp <container_name>:/nats-server ./build/bin/nats-server
chmod +x ./build/bin/nats-server
```

4. Verify it works:

```bash
./build/bin/nats-server --version
```

## Running the cnats dependency tests

The cnats library includes its own test suite. To run it, you need the `nats-server` binary available in your `PATH` (see section above).

### 2. Build the test suite

```bash
cmake --build build --target testsuite
```

The test executable is located at `build/bin/testsuite`.

### 3. Run a test

Make sure `nats-server` is in your `PATH`, then run a test by name:

```bash
PATH="$(pwd)/build/bin:$PATH" ./build/bin/testsuite <TestName>
```

For example:

```bash
PATH="$(pwd)/build/bin:$PATH" ./build/bin/testsuite ConnectedServer
```

### 4. List available tests

Test names are listed in `build/_deps/cnats-src/test/list_test.txt`. Each entry has the format `_test(TestName)` — use the name between parentheses as the argument.

## License

Licensed under the Apache License, Version 2.0. See the [LICENSE](http://www.apache.org/licenses/LICENSE-2.0) for details.
