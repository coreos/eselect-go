# eselect go

Manage multiple Go versions using Gentoo's eselect.

## Usage

This tool is intended for use in the CoreOS SDK in order to support
packages or architectures that cannot upgrade to the same Go version all
at the same time. Gentoo doesn't support this use case.

Go is expected to be installed to `/usr/lib/go1.5`, `/usr/lib/go1.6`,
etc. The `go` and `gofmt` binaries `/usr/bin` are actually wrappers and
select which version to execute based on the `EGO` environment variable,
the selection made by `eselect go`, or the latest version found.

    $ eselect go
    Usage: eselect go <action> <options>

    Standard actions:
      help                      Display help text
      usage                     Display usage information
      version                   Display version information

    Extra actions:
      list                      List installed Go versions
      set <target>              Set main active Go version
      show                      Show main active Go version
      update                    Switch to the latest Go version
        --if-unset                Do not override existing selection
        --ignore SLOT             Ignore SLOT when setting symlinks

Switch the default Go version:

    $ eselect go set go1.6
    $ go version
    go version go1.6.3 linux/amd64

Override the default in the environment:

    $ export EGO=go1.5
    $ go version
    go version go1.5.3 linux/amd64

## Bugs

Please use the [CoreOS issue tracker](https://github.com/coreos/bugs/issues)
