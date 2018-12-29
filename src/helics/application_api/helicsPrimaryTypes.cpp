/*
Copyright © 2017-2018,
Battelle Memorial Institute; Lawrence Livermore National Security, LLC; Alliance for Sustainable Energy, LLC
All rights reserved. See LICENSE file and DISCLAIMER for more details.
*/

#include "HelicsPrimaryTypes.hpp"
#include "ValueConverter.hpp"

#include <set>
namespace helics
{
bool changeDetected (const defV &prevValue, const std::string &val, double /*deltaV*/)
{
    if (prevValue.index () == string_loc)
    {
        return (val != mpark::get<std::string> (prevValue));
    }
    return true;
}

bool changeDetected (const defV &prevValue, const char *val, double /*deltaV*/)
{
    if (prevValue.index () == string_loc)
    {
        return (mpark::get<std::string> (prevValue) != val);
    }
    return true;
}

static const std::set<std::string> falseString{"0", "",   "false", "False", "FALSE", "f",  "F",
                                               "0", "\0", " ",     "no",    "NO",    "No", "-"};
static bool isTrueString (const std::string &str)
{
    if (str == "1")
    {
        return true;
    }
    if (str == "0")
    {
        return false;
    }
    return (falseString.find (str) != falseString.end ());
}

bool changeDetected (const defV &prevValue, bool val, double /*deltaV*/)
{
    if (prevValue.index () == string_loc)
    {
        return (isTrueString (mpark::get<std::string> (prevValue)) != val);
    }
    if (prevValue.index () == int_loc)
    {
        return ((mpark::get<int64_t> (prevValue) != 0) != val);
    }
    return true;
}

bool changeDetected (const defV &prevValue, const std::vector<double> &val, double deltaV)
{
    if (prevValue.index () == vector_loc)
    {
        const auto &prevV = mpark::get<std::vector<double>> (prevValue);
        if (val.size () == prevV.size ())
        {
            for (size_t ii = 0; ii < val.size (); ++ii)
            {
                if (std::abs (prevV[ii] - val[ii]) > deltaV)
                {
                    return true;
                }
            }
            return false;
        }
    }
    return true;
}

bool changeDetected (const defV &prevValue, const std::vector<std::complex<double>> &val, double deltaV)
{
    if (prevValue.index () == complex_vector_loc)
    {
        const auto &prevV = mpark::get<std::vector<std::complex<double>>> (prevValue);
        if (val.size () == prevV.size ())
        {
            for (size_t ii = 0; ii < val.size (); ++ii)
            {
                if (std::abs (prevV[ii] - val[ii]) > deltaV)
                {
                    return true;
                }
            }
            return false;
        }
    }
    return true;
}

bool changeDetected (const defV &prevValue, const double *vals, size_t size, double deltaV)
{
    if (prevValue.index () == vector_loc)
    {
        const auto &prevV = mpark::get<std::vector<double>> (prevValue);
        if (size == prevV.size ())
        {
            for (size_t ii = 0; ii < size; ++ii)
            {
                if (std::abs (prevV[ii] - vals[ii]) > deltaV)
                {
                    return true;
                }
            }
            return false;
        }
    }
    return true;
}

bool changeDetected (const defV &prevValue, const std::complex<double> &val, double deltaV)
{
    if (prevValue.index () == complex_loc)
    {
        const auto &prevV = mpark::get<std::complex<double>> (prevValue);
        if (std::abs (prevV.real () - val.real ()) > deltaV)
        {
            return true;
        }
        if (std::abs (prevV.imag () - val.imag ()) > deltaV)
        {
            return true;
        }
        return false;
    }
    return true;
}

bool changeDetected (const defV &prevValue, double val, double deltaV)
{
    if (prevValue.index () == double_loc)
    {
        return (std::abs (mpark::get<double> (prevValue) - val) > deltaV);
    }
    return true;
}

bool changeDetected (const defV &prevValue, Time val, double deltaV)
{
    if (prevValue.index () == double_loc)
    {
        return (std::abs (mpark::get<double> (prevValue) - static_cast<double> (val)) > deltaV);
    }
    else if (prevValue.index () == int_loc)
    {
        return (std::abs (Time (mpark::get<int64_t> (prevValue), time_units::ns) - val) > deltaV);
    }
    return true;
}

bool changeDetected (const defV &prevValue, int64_t val, double deltaV)
{
    if (prevValue.index () == int_loc)
    {
        return (std::abs (mpark::get<int64_t> (prevValue) - val) > static_cast<int64_t> (deltaV));
    }
    return true;
}

bool changeDetected (const defV &prevValue, const named_point &val, double deltaV)
{
    if ((prevValue.index () == double_loc) && (!std::isnan (val.value)))
    {
        return (std::abs (mpark::get<double> (prevValue) - val.value) > deltaV);
    }
    if (prevValue.index () == named_point_loc)
    {
        if ((mpark::get<named_point> (prevValue).name == val.name) && (!std::isnan (val.value)))
        {
            return (std::abs (mpark::get<named_point> (prevValue).value - val.value) > deltaV);
        }
    }
    return true;
}

void valueExtract (const defV &dv, std::string &val)
{
    switch (dv.index ())
    {
    case double_loc:  // double
        val = std::to_string (mpark::get<double> (dv));
        break;
    case int_loc:  // int64_t
        val = std::to_string (mpark::get<int64_t> (dv));
        break;
    case string_loc:  // string
    default:
        val = mpark::get<std::string> (dv);
        break;
    case complex_loc:  // complex
        val = helicsComplexString (mpark::get<std::complex<double>> (dv));
        break;
    case vector_loc:  // vector
        val = helicsVectorString (mpark::get<std::vector<double>> (dv));
        break;
    case complex_vector_loc:  // vector
        val = helicsComplexVectorString (mpark::get<std::vector<std::complex<double>>> (dv));
        break;
    case named_point_loc:
    {
        auto &np = mpark::get<named_point> (dv);
        val = (std::isnan (np.value)) ? np.name : helicsNamedPointString (np);
        break;
    }
    }
}

void valueExtract (const defV &dv, std::complex<double> &val)
{
    switch (dv.index ())
    {
    case double_loc:  // double
        val = std::complex<double> (mpark::get<double> (dv), 0.0);
        break;
    case int_loc:  // int64_t
        val = std::complex<double> (static_cast<double> (mpark::get<int64_t> (dv)), 0.0);
        break;
    case string_loc:  // string
    default:
        val = getComplexFromString (mpark::get<std::string> (dv));
        break;
    case complex_loc:  // complex
        val = mpark::get<std::complex<double>> (dv);
        break;
    case vector_loc:  // vector
    {
        auto &vec = mpark::get<std::vector<double>> (dv);
        if (vec.size () == 1)
        {
            val = std::complex<double> (vec[0], 0.0);
        }
        else if (vec.size () > 2)
        {
            val = std::complex<double> (vec[0], vec[1]);
        }
        break;
    }
    case complex_vector_loc:
    {
        auto &vec = mpark::get<std::vector<std::complex<double>>> (dv);
        if (!vec.empty ())
        {
            val = vec.front ();
        }
        break;
    }
    case named_point_loc:
    {
        auto &np = mpark::get<named_point> (dv);
        if (std::isnan (np.value))
        {
            val = getComplexFromString (np.name);
        }
        else
        {
            val = std::complex<double> (np.value, 0.0);
        }
    }
    break;
    }
}

void valueExtract (const defV &dv, std::vector<double> &val)
{
    val.resize (0);
    switch (dv.index ())
    {
    case double_loc:  // double
        val.push_back (mpark::get<double> (dv));
        break;
    case int_loc:  // int64_t
        val.push_back (static_cast<double> (mpark::get<int64_t> (dv)));
        break;
    case string_loc:  // string
    default:
        helicsGetVector (mpark::get<std::string> (dv), val);
        break;
    case complex_loc:  // complex
    {
        auto cval = mpark::get<std::complex<double>> (dv);
        val.push_back (cval.real ());
        val.push_back (cval.imag ());
    }
    break;
    case vector_loc:  // vector
        val = mpark::get<std::vector<double>> (dv);
        break;
    case complex_vector_loc:  // complex
    {
        auto &cv = mpark::get<std::vector<std::complex<double>>> (dv);
        val.resize (2 * cv.size ());
        for (auto &cval : cv)
        {
            val.push_back (cval.real ());
            val.push_back (cval.imag ());
        }
    }
    break;
    case named_point_loc:  // named point
    {
        auto &np = mpark::get<named_point> (dv);
        if (std::isnan (np.value))
        {
            val = helicsGetVector (np.name);
        }
        else
        {
            val.resize (1);
            val[0] = np.value;
        }
        break;
    }
    }
}

void valueExtract (const defV &dv, std::vector<std::complex<double>> &val)
{
    val.resize (0);
    switch (dv.index ())
    {
    case double_loc:  // double
        val.emplace_back (mpark::get<double> (dv), 0.0);
        break;
    case int_loc:  // int64_t
        val.emplace_back (static_cast<double> (mpark::get<int64_t> (dv)), 0.0);
        break;
    case string_loc:  // string
    default:
        helicsGetComplexVector (mpark::get<std::string> (dv), val);
        break;
    case complex_loc:  // complex
    {
        val.push_back (mpark::get<std::complex<double>> (dv));
    }
    break;
    case vector_loc:  // vector
    {
        auto &v = mpark::get<std::vector<double>> (dv);
        val.resize (v.size () / 2);
        for (size_t ii = 0; ii < v.size () - 1; ii += 2)
        {
            val.emplace_back (v[ii], v[ii + 1]);
        }
        break;
    }
    case complex_vector_loc:  // complex
        val = mpark::get<std::vector<std::complex<double>>> (dv);
        break;
    case named_point_loc:  // named point
    {
        auto &np = mpark::get<named_point> (dv);
        if (std::isnan (np.value))
        {
            val = helicsGetComplexVector (np.name);
        }
        else
        {
            val.resize (1);
            val[0] = std::complex<double> (np.value, 0.0);
        }
        break;
    }
    }
}

void valueExtract (const defV &dv, named_point &val)
{
    switch (dv.index ())
    {
    case double_loc:  // double
        val.name = "value";
        val.value = mpark::get<double> (dv);
        break;
    case int_loc:  // int64_t
        val.name = "value";
        val.value = static_cast<double> (mpark::get<int64_t> (dv));
        break;
    case string_loc:  // string
    default:
        val = helicsGetNamedPoint (mpark::get<std::string> (dv));
        break;
    case complex_loc:  // complex
        val.name = helicsComplexString (mpark::get<std::complex<double>> (dv));
        val.value = std::nan ("0");
        break;
    case vector_loc:  // vector
    {
        auto &vec = mpark::get<std::vector<double>> (dv);
        if (vec.size () == 1)
        {
            val.name = "value";
            val.value = vec[0];
        }
        else
        {
            val.name = helicsVectorString (vec);
            val.value = std::nan ("0");
        }

        break;
    }
    case complex_vector_loc:
    {
        val.value = std::nan ("0");
        auto &vec = mpark::get<std::vector<std::complex<double>>> (dv);
        if (vec.size () == 1)
        {
            val.name = helicsComplexString (vec[0]);
        }
        else
        {
            val.name = helicsComplexVectorString (vec);
        }
        break;
    }
    case named_point_loc:
        val = mpark::get<named_point> (dv);
        break;
    }
}

void valueExtract (const defV &dv, Time &val)
{
    switch (dv.index ())
    {
    case double_loc:  // double
        val = mpark::get<double> (dv);
        break;
    case int_loc:  // int64_t
    default:
        val.setBaseTimeCode (mpark::get<int64_t> (dv));
        break;
    case string_loc:  // string
    {
        size_t index;
        auto &str = mpark::get<std::string> (dv);
        try
        {
            auto ul = std::stoll (str, &index);
            if ((index == std::string::npos) || (index == str.size ()))
            {
                val.setBaseTimeCode (ul);
            }
            else
            {
                val = loadTimeFromString (mpark::get<std::string> (dv));
            }
        }
        catch (...)
        {
            val = timeZero;
        }
        break;
    }
    case complex_loc:  // complex
        val = mpark::get<std::complex<double>> (dv).real ();
        break;
    case vector_loc:  // vector
    {
        auto &vec = mpark::get<std::vector<double>> (dv);
        if (vec.size () >= 1)
        {
            val = vec[0];
        }
        else
        {
            val = timeZero;
        }

        break;
    }
    case complex_vector_loc:
    {
        auto &vec = mpark::get<std::vector<std::complex<double>>> (dv);
        if (vec.size () >= 1)
        {
            val = vec[0].real ();
        }
        else
        {
            val = timeZero;
        }
        break;
    }
    case named_point_loc:
        val = mpark::get<named_point> (dv).value;
        break;
    }
}

void valueExtract (const defV &dv, char &val)
{
    switch (dv.index ())
    {
    case double_loc:  // double
        val = static_cast<char> (mpark::get<double> (dv));
        break;
    case int_loc:  // int64_t
    default:
        val = static_cast<char> (mpark::get<int64_t> (dv));
        break;
    case string_loc:  // string
    {
        auto &str = mpark::get<std::string> (dv);
        val = (str.empty ()) ? '\0' : str[0];
        break;
    }
    case complex_loc:  // complex
        val = static_cast<char> (mpark::get<std::complex<double>> (dv).real ());
        break;
    case vector_loc:  // vector
    {
        auto &vec = mpark::get<std::vector<double>> (dv);
        if (vec.size () >= 1)
        {
            val = static_cast<char> (vec[0]);
        }
        else
        {
            val = '\0';
        }

        break;
    }
    case complex_vector_loc:
    {
        auto &vec = mpark::get<std::vector<std::complex<double>>> (dv);
        if (vec.size () >= 1)
        {
            val = static_cast<char> (vec[0].real ());
        }
        else
        {
            val = '\0';
        }
        break;
    }
    case named_point_loc:
    {
        auto &np = mpark::get<named_point> (dv);
        val = np.name.empty () ? (static_cast<char> (np.value)) : np.name[0];
    }
    break;
    }
}

void valueExtract (const data_view &dv, data_type baseType, std::string &val)
{
    switch (baseType)
    {
    case data_type::helicsDouble:
    {
        auto V = ValueConverter<double>::interpret (dv);
        val = std::to_string (V);
        break;
    }
    case data_type::helicsInt:
    case data_type::helicsTime:
    {
        auto V = ValueConverter<int64_t>::interpret (dv);
        val = std::to_string (V);
        break;
    }
    case data_type::helicsString:
    default:
        val = dv.string ();
        break;
    case data_type::helicsNamedPoint:
        val = helicsNamedPointString (ValueConverter<named_point>::interpret (dv));
        break;
    case data_type::helicsVector:
        val = helicsVectorString (ValueConverter<std::vector<double>>::interpret (dv));
        break;
    case data_type::helicsComplex:
        val = helicsComplexString (ValueConverter<std::complex<double>>::interpret (dv));
        break;
    case data_type::helicsComplexVector:
        val = helicsComplexVectorString (ValueConverter<std::vector<std::complex<double>>>::interpret (dv));
        break;
    }
}

void valueExtract (const data_view &dv, data_type baseType, std::vector<double> &val)
{
    val.resize (0);
    switch (baseType)
    {
    case data_type::helicsDouble:
    {
        val.push_back (ValueConverter<double>::interpret (dv));
        break;
    }
    case data_type::helicsInt:
    {
        val.push_back (static_cast<double> (ValueConverter<int64_t>::interpret (dv)));
        break;
    }
    case data_type::helicsTime:
    {
        Time tm (ValueConverter<int64_t>::interpret (dv), time_units::ns);
        val.push_back (static_cast<double> (tm));
    }
    break;
    case data_type::helicsString:
    default:
    {
        helicsGetVector (dv.string (), val);
        break;
    }
    case data_type::helicsNamedPoint:
    {
        auto npval = ValueConverter<named_point>::interpret (dv);
        if (std::isnan (npval.value))
        {
            val = helicsGetVector (dv.string ());
        }
        else
        {
            val.push_back (npval.value);
        }
        break;
    }
    case data_type::helicsVector:
    {
        ValueConverter<std::vector<double>>::interpret (dv, val);
        break;
    }
    case data_type::helicsComplex:
    {
        auto cval = ValueConverter<std::complex<double>>::interpret (dv);
        val.push_back (cval.real ());
        val.push_back (cval.imag ());
        break;
    }
    case data_type::helicsComplexVector:
    {
        auto cv = ValueConverter<std::vector<std::complex<double>>>::interpret (dv);
        val.reserve (2 * cv.size ());
        for (auto &cval : cv)
        {
            val.push_back (cval.real ());
            val.push_back (cval.imag ());
        }
        break;
    }
    }
}

void valueExtract (const data_view &dv, data_type baseType, std::vector<std::complex<double>> &val)
{
    val.resize (0);
    switch (baseType)
    {
    case data_type::helicsDouble:
    {
        val.emplace_back (ValueConverter<double>::interpret (dv), 0.0);
        break;
    }
    case data_type::helicsInt:
    {
        val.emplace_back (static_cast<double> (ValueConverter<int64_t>::interpret (dv)), 0.0);
        break;
    }
    case data_type::helicsTime:
    {
        Time tm (ValueConverter<int64_t>::interpret (dv), time_units::ns);
        val.emplace_back (static_cast<double> (tm), 0.0);
    }
    break;
    case data_type::helicsString:
    default:
    {
        helicsGetComplexVector (dv.string (), val);
        break;
    }
    case data_type::helicsVector:
    {
        auto V = ValueConverter<std::vector<double>>::interpret (dv);
        for (size_t ii = 0; ii < V.size () - 1; ii += 2)
        {
            val.emplace_back (V[ii], V[ii + 1]);
        }
        break;
    }
    case data_type::helicsComplexVector:
    {
        ValueConverter<std::vector<std::complex<double>>>::interpret (dv, val);
        break;
    }
    case data_type::helicsNamedPoint:
    {
        auto npval = ValueConverter<named_point>::interpret (dv);
        if (std::isnan (npval.value))
        {
            val = helicsGetComplexVector (npval.name);
        }
        else
        {
            val.emplace_back (npval.value, 0.0);
        }
        break;
    }
    case data_type::helicsComplex:
    {
        auto cval = ValueConverter<std::complex<double>>::interpret (dv);
        val.push_back (cval);
        break;
    }
    }
}

void valueExtract (const data_view &dv, data_type baseType, std::complex<double> &val)
{
    switch (baseType)
    {
    case data_type::helicsDouble:
    {
        val = std::complex<double> (ValueConverter<double>::interpret (dv), 0.0);
        break;
    }
    case data_type::helicsInt:
    {
        val = std::complex<double> (static_cast<double> (ValueConverter<int64_t>::interpret (dv)), 0.0);
        break;
    }
    case data_type::helicsTime:
    {
        Time tm (ValueConverter<int64_t>::interpret (dv), time_units::ns);
        val = std::complex<double> (static_cast<double> (tm), 0.0);
    }
    break;
    case data_type::helicsString:
    default:
    {
        val = helicsGetComplex (dv.string ());
        break;
    }
    case data_type::helicsNamedPoint:
    {
        auto npval = ValueConverter<named_point>::interpret (dv);
        if (std::isnan (npval.value))
        {
            val = helicsGetComplex (npval.name);
        }
        else
        {
            val = std::complex<double> (npval.value, 0.0);
        }
        break;
    }
    case data_type::helicsVector:
    {
        auto vec = ValueConverter<std::vector<double>>::interpret (dv);
        if (vec.size () == 1)
        {
            val = std::complex<double> (vec[0], 0.0);
        }
        else if (vec.size () > 2)
        {
            val = std::complex<double> (vec[0], vec[1]);
        }
        break;
    }
    case data_type::helicsComplex:
    {
        val = ValueConverter<std::complex<double>>::interpret (dv);
        break;
    }
    }
}

void valueExtract (const data_view &dv, data_type baseType, named_point &val)
{
    switch (baseType)
    {
    case data_type::helicsDouble:
    {
        auto V = ValueConverter<double>::interpret (dv);
        val.name = "value";
        val.value = V;
        break;
    }
    case data_type::helicsInt:
    {
        auto V = ValueConverter<int64_t>::interpret (dv);
        val.name = "value";
        val.value = static_cast<double> (V);
        break;
    }
    case data_type::helicsTime:
    {
        Time tm (ValueConverter<int64_t>::interpret (dv), time_units::ns);
        val.name = "time";
        val.value = static_cast<double> (tm);
    }
    break;
    case data_type::helicsString:
    default:
    {
        val = helicsGetNamedPoint (dv.string ());
        break;
    }
    case data_type::helicsVector:
    {
        auto vec = ValueConverter<std::vector<double>>::interpret (dv);
        if (vec.size () == 1)
        {
            val.name = "value";

            val.value = vec[0];
        }
        else
        {
            val.name = helicsVectorString (vec);
            val.value = std::nan ("0");
        }
        break;
    }
    case data_type::helicsComplex:
    {
        auto cval = ValueConverter<std::complex<double>>::interpret (dv);
        if (cval.imag () == 0)
        {
            val.name = "value";
            val.value = cval.real ();
        }
        else
        {
            val.name = helicsComplexString (cval);
            val.value = std::nan ("0");
        }

        break;
    }
    case data_type::helicsComplexVector:
    {
        auto cvec = ValueConverter<std::vector<std::complex<double>>>::interpret (dv);
        if (cvec.size () == 1)
        {
            val.name = helicsComplexString (cvec[0]);
            val.value = std::nan ("0");
        }
        else
        {
            val.name = helicsComplexVectorString (cvec);
            val.value = std::nan ("0");
        }
        break;
    }
    case data_type::helicsNamedPoint:
        val = ValueConverter<named_point>::interpret (dv);
        break;
    }
}

void valueExtract (const data_view &dv, data_type baseType, Time &val)
{
    switch (baseType)
    {
    case data_type::helicsDouble:
    {
        val = ValueConverter<double>::interpret (dv);
        break;
    }
    case data_type::helicsInt:
    case data_type::helicsTime:
    {
        val.setBaseTimeCode (ValueConverter<int64_t>::interpret (dv));
        break;
    }
    case data_type::helicsString:
    default:
    {
        size_t index;
        try
        {
            auto ul = std::stoll (dv.string (), &index);
            if ((index == std::string::npos) || (index == dv.string ().size ()))
            {
                val.setBaseTimeCode (ul);
            }
            else
            {
                val = loadTimeFromString (dv.string ());
            }
        }
        catch (...)
        {
            val = timeZero;
        }

        break;
    }
    case data_type::helicsVector:
    {
        auto vec = ValueConverter<std::vector<double>>::interpret (dv);
        if (vec.size () >= 1)
        {
            val = vec[0];
        }
        else
        {
            val = timeZero;
        }
        break;
    }
    case data_type::helicsComplex:
    {
        auto cval = ValueConverter<std::complex<double>>::interpret (dv);
        val = cval.real ();
        break;
    }
    case data_type::helicsComplexVector:
    {
        auto cvec = ValueConverter<std::vector<std::complex<double>>>::interpret (dv);
        if (cvec.size () >= 1)
        {
            val = cvec[0].real ();
        }
        else
        {
            val = timeZero;
        }
        break;
    }
    case data_type::helicsNamedPoint:
    {
        auto np = ValueConverter<named_point>::interpret (dv);
        val = np.value;
        break;
    }
    }
}

void valueExtract (const data_view &dv, data_type baseType, defV &val)
{
    switch (baseType)
    {
    case data_type::helicsDouble:
        val = ValueConverter<double>::interpret (dv);
        break;
    case data_type::helicsInt:
    case data_type::helicsTime:
        val = ValueConverter<int64_t>::interpret (dv);
        break;
    case data_type::helicsString:
    default:
        val = dv.string ();
        break;
    case data_type::helicsVector:
        val = ValueConverter<std::vector<double>>::interpret (dv);
        break;
    case data_type::helicsComplex:
        val = ValueConverter<std::complex<double>>::interpret (dv);
        break;
    case data_type::helicsComplexVector:
        val = ValueConverter<std::vector<std::complex<double>>>::interpret (dv);
        break;
    case data_type::helicsNamedPoint:
        val = ValueConverter<named_point>::interpret (dv);
        break;
    }
}

void valueConvert (defV &val, data_type newType)
{
    auto index = val.index ();
    switch (newType)
    {
    case data_type::helicsDouble:
    {
        if (index == double_loc)
        {
            return;
        }
        double V;
        valueExtract (val, V);
        val = V;
        break;
    }
    case data_type::helicsInt:
    {
        if (index == int_loc)
        {
            return;
        }
        int64_t V;
        valueExtract (val, V);
        val = V;
        break;
    }
    case data_type::helicsTime:
    {
        if (index == int_loc)
        {
            return;
        }
        Time V;
        valueExtract (val, V);
        val = V.getBaseTimeCode ();
        break;
        break;
    }
    case data_type::helicsString:
    default:
    {
        if (index == string_loc)
        {
            return;
        }
        std::string V;
        valueExtract (val, V);
        val = std::move (V);
        break;
    }
    case data_type::helicsVector:
    {
        if (index == vector_loc)
        {
            return;
        }
        std::vector<double> V;
        valueExtract (val, V);
        val = std::move (V);
        break;
    }
    case data_type::helicsComplex:
    {
        if (index == complex_loc)
        {
            return;
        }
        std::complex<double> V;
        valueExtract (val, V);
        val = V;
        break;
    }
    case data_type::helicsComplexVector:
    {
        if (index == complex_vector_loc)
        {
            return;
        }
        std::vector<std::complex<double>> V;
        valueExtract (val, V);
        val = std::move (V);
        break;
    }
    case data_type::helicsNamedPoint:
    {
        if (index == named_point_loc)
        {
            return;
        }
        named_point V;
        valueExtract (val, V);
        val = std::move (V);
        break;
    }
    }
}

}  // namespace helics
