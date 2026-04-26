export module Atl;

export import :Algorithm;
export import :Array;
export import :Bit;
export import :DateTime;
export import :Def;
export import :Digest;
export import :Exception;
export import :File;
export import :Math;
export import :Memory;
export import :String;

export
{
  Atl::Int Main(Atl::Int argc, Atl::Char* argv[]);
  Atl::Int main(Atl::Int argc, Atl::Char* argv[]) { std::ios::sync_with_stdio(Atl::False); return Main(argc, argv); }
}
