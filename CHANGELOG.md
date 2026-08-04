# Changelog

## v2.2.2
* Added license for Doxygen Awesome.
    * ***IMPORTANT:*** This was mistakenly forgotten in previous versions of 
    EasySockets so if you wish to use an older version please add the new 
    license file to your project.

## v2.2.1
* Fixed bug where `es::Socket` default constructor was private.

## v2.2.0
* Added `es::Socket::close`.

## v2.1.0
* Added `es::Socket::make_connected_tcp`.
* Added `es::Socket::make_connected_udp`.
* Added `es::Socket::make_bound_tcp`.
* Added `es::Socket::make_bound_udp`.

## v2.0.0
* Changed naming conventions for enumerations.
* Added `shutdown` method for `es::Context`.