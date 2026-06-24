export module Atl;

export import :Algorithm;
export import :Allocator;
export import :Array;
export import :Bit;
export import :Concepts;
export import :DateTime;
export import :Def;
export import :Digest;
export import :Exception;
export import :File;
export import :InitializerList;
export import :Limits;
export import :Math;
export import :Memory;
export import :String;
export import :Type;
export import :Vector;

export
{
  Atl::Int Main(Atl::Int argc, Atl::Char* argv[]);

  Atl::Int main(Atl::Int argc, Atl::Char* argv[])
  {
    std::ios::sync_with_stdio(false);
    return Main(argc, argv);
  }
}
