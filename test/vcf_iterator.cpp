// tested
#include <vcf_iterator.hpp>

// local
#include <vcf.hpp>

// boost
#include <boost/test/unit_test.hpp>

// std
#include <sstream>


using namespace vcf;


BOOST_AUTO_TEST_SUITE(vcf_iterator)


BOOST_AUTO_TEST_CASE(end_equality)
{
    BOOST_CHECK(VcfIterator() == VcfIterator());
}


BOOST_AUTO_TEST_CASE(non_equality)
{
    Vcf parser1({}, {});
    Vcf parser2({}, {});
    BOOST_CHECK(VcfIterator(parser1) != VcfIterator(parser2));
}


BOOST_AUTO_TEST_CASE(empty_equls_end)
{
    Vcf parser{{}, {}};
    BOOST_CHECK(VcfIterator(parser) == VcfIterator());
}


BOOST_AUTO_TEST_CASE(initial_state_error_is_end)
{
    std::istringstream ins{"f"};

    Vcf parser(
        std::istreambuf_iterator<char>{ins},
        std::istreambuf_iterator<char>{});

    BOOST_CHECK(VcfIterator(parser) == VcfIterator());
}


BOOST_AUTO_TEST_CASE(complex)
{
    std::istringstream ins{
        "BEGIN:VCARD\n"
        "ADR;TYPE=cell,home;CHARSET=UTF-8;ENCODING=QUOTED-PRINTABLE:;;;;;;\n"
        "END:VCARD\n"
        "BEGIN:VCARD\n"
        "ADR;TYPE=cell,home;CHARSET=UTF-8;ENCODING=QUOTED-PRINTABLE:;;;;;;\n"
        "END:VCARD"
    };

    Vcf parser(
        std::istreambuf_iterator<char>{ins},
        std::istreambuf_iterator<char>{});

    VcfIterator it{parser};
    const auto end = VcfIterator();
    BOOST_CHECK(it != end);

    {
        VCard x = *it;
    
        BOOST_CHECK(!parser.error());
        BOOST_CHECK(!parser.eof());
    
        BOOST_CHECK(x.address->type);
        BOOST_CHECK(x.address->charset);
        BOOST_CHECK(x.address->encoding);
    
        BOOST_CHECK_EQUAL("cell,home", *x.address->type);
    
        BOOST_CHECK_EQUAL("UTF-8", *x.address->charset);
        BOOST_CHECK_EQUAL("QUOTED-PRINTABLE", *x.address->encoding);
    }

    ++it;
    BOOST_CHECK(it != end);

    {
        VCard x = *it;
    
        BOOST_CHECK(!parser.error());
        BOOST_CHECK(parser.eof());
    
        BOOST_CHECK(x.address->type);
        BOOST_CHECK(x.address->charset);
        BOOST_CHECK(x.address->encoding);
    
        BOOST_CHECK_EQUAL("cell,home", *x.address->type);
    
        BOOST_CHECK_EQUAL("UTF-8", *x.address->charset);
        BOOST_CHECK_EQUAL("QUOTED-PRINTABLE", *x.address->encoding);
    }

    ++it;
    BOOST_CHECK(it == end);
}


BOOST_AUTO_TEST_SUITE_END()