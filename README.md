# plaid-cc

A C++ client library for the [Plaid API](https://plaid.com/docs).

## Table of Contents

- [plaid-cc](#plaid-cc)
  * [Introduction](#introduction)
  * [Install](#install)
  * [Usage](#usage)
  * [Contributing](#contributing)
  * [License](#license)

## Introduction

As of January 13th, 2020, Plaid was [acquired by Visa](https://blog.plaid.com/plaid-and-visa/) for a whopping $5.3 billion dollars. A few days later, [Zach](https://twitter.com/zachperret) (the CEO and Co-Founder) of Plaid tweeted this:

> Oh, and also ... we are very much still hiring. We've just finished the prologue to the @Plaid journey. Join us for Chapter 1!

To me, this was pretty exciting. I've used Plaid in the past for personal projects, and I've followed Zach on Twitter for a good while now. The company has an incredible product... why? It's because they've built a road for so many other developers & entrepreneurs to securely interact with users bank accounts. The banking industry as a whole has been stuck in the past for a while now, and Plaid has been a pioneer in advancing this industry forward. 

So... why did I build this library? Three reasons: giving back / contributing, the free t-shirt, and because Plaid doesn't have any job postings for summer interns. History has shown that underestimating enthusiastic sophomores is probably a bad idea (thanks Mark!), so if one of these students writes an entire client library in a single night, imagine what they could do in four months.

On a side note, a possible fourth reason why I wanted to build this library was that I could see how effective [Protocol Buffers](https://developers.google.com/protocol-buffers) were at JSON serialization/deserialization. Turns out, they're pretty good. The best part about using this type of approach is the portability, since protocol buffers can be compiled to a number of different languages. This way, a single protocol buffer file can act as a single point of truth for the entire api, which means that the effort of maintaining client libraries becomes significantly easier.

## Install

This project uses the [Meson Build](https://mesonbuild.com/) system. To build and install it, simply do the following:
```
git clone git@github.com:MorganGallant/plaid-cc.git
cd plaid-cc
meson build
cd build
ninja install
```
This will copy all of the headers to your include path, copy the shared library file to your lib path, and add the library to your package config path.

## Usage

After installation, usage should be really straightforward. This library was modelled after the [plaid-go](https://github.com/plaid/plaid-go) library, and the function names / usage should be close to identical.
```
#include <iostream>
#include <plaid/client.h>

int main() {
  // Create the client from a set of credentials.
  auto creds = plaid::Credentials(plaid::Enviroment::Sandbox, "client-id-here",
                                  "public-key-here", "secret-key-here");
  auto client = plaid::Client::Create(creds);

  // Do a test api call, say, get all categories.
  auto categories = client->GetCategories();
  if (!categories.IsOk()) {
    std::cerr << "Error: " << categories.DescribeStatus() << std::endl;
    return 1;
  }
  // In order to access the data, the categories response must be unwrapped.
  std::cout << "Got " << categories.Unwrap().categories_size() << "categories.";
  std::cout << std::endl;
}
```
One thing to note, the `plaid::Client::Create(creds)` call returns an `std::unique_ptr<plaid::Client>` object. This means that you do not have to explicitly cleanup the object once you are finished with it.

The library should support all current API methods, including the payments API added which was added yesterday to the [payment library](https://github.com/plaid/plaid-go/commit/d03fb88d4319479fd062f3c26b34b701477a7e9f) committed on Jan 17th (two days ago as of writing this).

## Contributing

Contributions are welcome to this repository, especially those which ensure that the API methods are up to date. I'll try my best to monitor recent API changes, but I may not always be able to update the codebase instantly to reflect the new changes. At a minimum, I will try to reflect any changes to the `plaid-go` repository.

## License

This project is under the MIT license, which was deliberately chosen to ensure that companies and programmers can use this library in their closed source software with no issues. Plaid is all about enabling people to do amazing things in the fintech industry, and my goal will always be to support those people.
