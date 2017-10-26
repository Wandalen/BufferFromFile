#include <boost/test/unit_test.hpp>

#include <vector>
#include <array>
#include <numeric>


#include "./TypedBuffer.hpp"

BOOST_AUTO_TEST_SUITE(  TypedBufferStd );
BOOST_AUTO_TEST_CASE( case1 )
{
  std::vector<int> vec;
  wTypedBufferStd<int> buf( vec.data(), vec.size() );
  BOOST_CHECK_EQUAL( buf.empty(), true );
  BOOST_CHECK_EQUAL( buf.size(), 0 );

}

//

BOOST_AUTO_TEST_CASE( case2 )
{
  std::vector<int> vec( 5 );
  vec = { 1, 2 };
  wTypedBufferStd<int> buf( vec.data(), vec.size() );
  BOOST_CHECK_EQUAL( buf.size(), 2 );
  // BOOST_CHECK_EQUAL( vec.capacity(), 5 );
  BOOST_CHECK_EQUAL( std::accumulate( buf.begin(), buf.end(), 0 ), 3 );

}

//

BOOST_AUTO_TEST_CASE( case3 )
{
  std::vector<int> vec( 3, 2 );
  wTypedBufferStd<int> buf( vec.data(), vec.size() );
  BOOST_CHECK_EQUAL( buf.size(), 3 );
  BOOST_CHECK_EQUAL( buf[ 0 ], 2 );
  BOOST_CHECK_EQUAL( std::accumulate( buf.begin(), buf.end(), 0 ), 6 );

}

//

BOOST_AUTO_TEST_CASE( case4 )
{
  std::vector<int> vec( 10 );
  wTypedBufferStd<int> buf( vec.data(), vec.size() );
  BOOST_CHECK_EQUAL( buf.size(), 10 );
  BOOST_CHECK_EQUAL( buf[ 0 ], 0 );
  BOOST_CHECK_EQUAL( std::accumulate( buf.begin(), buf.end(), 0 ), 0 );

}

//

BOOST_AUTO_TEST_CASE( case5 )
{
  int* data = new int[ 2 ];
  data[ 0 ] = 1;
  data[ 1 ] = 2;
  wTypedBufferStd<int> buf( data, 2 );
  BOOST_CHECK_EQUAL( buf.size(), 2 );
  BOOST_CHECK_EQUAL( std::accumulate( buf.begin(), buf.end(), 0 ), std::accumulate( data, data + 2, 0 ) );
}

//

BOOST_AUTO_TEST_CASE( case6 )
{
  std::array<int,2> arr = { 1, 2 };
  wTypedBufferStd<int> buf( arr.data(), arr.size() );
  wTypedBufferStd<int> buf2( buf );

  BOOST_CHECK_EQUAL( buf == buf2, true );

  buf2[ 0 ] = 5;
  BOOST_CHECK_EQUAL( buf[ 0 ], 5 );
  BOOST_CHECK_EQUAL( buf2[ 0 ], arr[ 0 ] );
}

//

BOOST_AUTO_TEST_CASE( case7 )
{
  std::vector<int> v1 = { -1, -2, -3 };
  std::vector<int> v2 = { 0, 1, 2 };

  wTypedBufferStd<int> buf( v1.data(), v1.size() );
  wTypedBufferStd<int> buf2( v2.data(), v2.size() );

  BOOST_CHECK_EQUAL( buf != buf2, true );

  v2 = v1;

  BOOST_CHECK_EQUAL( v1 == v2, true );

  // v2.push_back( -1 );
  BOOST_CHECK_EQUAL( v1.size(),v2.size() );
  // BOOST_CHECK_EQUAL( v2.size(), 4 );
}

//

BOOST_AUTO_TEST_CASE( case8 )
{
  std::vector<int> v1 = { 0, 1, 2 };
  std::vector<int> v2 = { 3, 4, 5 };

  wTypedBufferStd<int> buf( v1.data(), v1.size() );
  wTypedBufferStd<int> buf2( v2.data(), v2.size() );

  std::swap( buf, buf2 );

  BOOST_CHECK_EQUAL( buf[ 0 ], 3 );
  BOOST_CHECK_EQUAL( buf2[ 0 ], 0 );

  BOOST_CHECK_EQUAL( &v1[ 0 ], &buf2[ 0 ] );
  BOOST_CHECK_EQUAL( &v2[ 0 ], &buf[ 0 ] );
}

//

BOOST_AUTO_TEST_CASE( case9 )
{
  std::vector<int> vec(2);
  vec = { 2, 3 };
  wTypedBufferStd<int> buf( vec.data(), vec.size() );

  BOOST_CHECK_EQUAL( buf.front(), 2 );
  BOOST_CHECK_EQUAL( buf.back(), vec.back() );
}

//

BOOST_AUTO_TEST_CASE( case10 )
{
  std::vector<int> vec = { 0, 1, 2 };
  wTypedBufferStd<int> buf( vec.data(), vec.size() );

  auto it = buf.begin();

  BOOST_CHECK_EQUAL( *it++, 0 );
  BOOST_CHECK_EQUAL( *it++, 1 );
  BOOST_CHECK_EQUAL( *it++, 2 );
}

//

BOOST_AUTO_TEST_CASE( case11 )
{
  std::vector<int> vec = { 1, 2 };
  wTypedBufferStd<int> buf( vec.data(), vec.size() );

  int* p = &vec[ 2 ];

  BOOST_CHECK( p == &buf[ 2 ] );
  *p = 5;
  BOOST_CHECK( buf[ 2 ] == 5 );
}

//

BOOST_AUTO_TEST_CASE( case12 )
{
  std::vector<int> vec = { 1, 2, 3 };
  wTypedBufferStd<int> buf( vec.data(), vec.size() );

  BOOST_CHECK( &( *buf.begin() ) == &( *vec.begin() ) );
  BOOST_CHECK( &( *buf.end() ) == &( *vec.end() ) );
}

//

BOOST_AUTO_TEST_CASE( case13 )
{
  std::vector<int> vec = { 0, 1, 2, 3, 4 };
  wTypedBufferStd<int> buf( vec.data(), vec.size() );

  int i = 0;
  for( auto iter = buf.begin(); iter != buf.end(); iter++ )
  {
   BOOST_CHECK( *iter == i  );
   *iter = ++i;
  }

  i = 1;
  for( auto iter = vec.cbegin(); iter != vec.cend(); iter++ )
  BOOST_CHECK( *iter == i++  );

  BOOST_CHECK( std::equal( buf.begin(), buf.end(), vec.begin(), vec.end() ) );

}

//

BOOST_AUTO_TEST_CASE( case14 )
{
  //fill all buffer
  wTypedBufferStd<int> buf( new Int32[ 3 ], 3 );
  buf.fill( 1 );
  std::vector<int> expected = { 1, 1, 1 };
  BOOST_CHECK( std::equal( buf.begin(), buf.end(), expected.begin(), expected.end() ) );

  //fill part of a buffer
  buf.fill( 2, buf.begin() + 1, buf.end() );
  expected = { 1, 2, 2 };
  BOOST_CHECK( std::equal( buf.begin(), buf.end(), expected.begin(), expected.end() ) );

  //end position is not specified
  buf.fill( 3, buf.begin() + 1 );
  expected = { 3, 3, 3 };
  BOOST_CHECK( std::equal( buf.begin(), buf.end(), expected.begin(), expected.end() ) );

  //begin position is not specified
  buf.fill( 4, NULL, buf.end() );
  expected = { 4, 4, 4 };
  BOOST_CHECK( std::equal( buf.begin(), buf.end(), expected.begin(), expected.end() ) );
}

BOOST_AUTO_TEST_SUITE_END()
