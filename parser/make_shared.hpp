#ifndef BOOST_PHOENIX_MAKE_SHARED_HPP
#define BOOST_PHOENIX_MAKE_SHARED_HPP

#include <memory>

#include <boost/phoenix/core/limits.hpp>
#include <boost/phoenix/core/call.hpp>
#include <boost/phoenix/core/expression.hpp>
#include <boost/phoenix/core/meta_grammar.hpp>
#include <boost/phoenix/object/detail/target.hpp>
#include <boost/phoenix/support/iterate.hpp>
#include <boost/preprocessor/repetition/repeat_from_to.hpp>
#include <boost/preprocessor/repetition/enum_shifted_binary_params.hpp>

BOOST_PHOENIX_DEFINE_EXPRESSION_VARARG(
    (boost)(phoenix)(make_shared)
  , (proto::terminal<detail::target<proto::_> >)
    (meta_grammar)
  , BOOST_PHOENIX_COMPOSITE_LIMIT
)

namespace boost { namespace phoenix
{
    struct make_shared_eval
    {
        template <typename Sig>
        struct result;

        template <typename This, typename A0,typename Context>
        struct result<This(A0, Context)>
        {
            typedef std::shared_ptr<typename detail::result_of::target<A0>::type > type;
        };

        template <typename Target, typename Context>
        typename std::shared_ptr<typename detail::result_of::target<Target>::type>
        operator()(Target, Context const &) const
        {
            return std::make_shared<typename detail::result_of::target<Target>::type>();
        }

        #define EVAL_a(_,N,__)                                                                \
            BOOST_PP_COMMA_IF(BOOST_PP_DEC(N)) boost::phoenix::eval(a ## N, ctx)

        #define GENERATE_MAKE_SHARED_EVAL(_,N,__)                                             \
        template <typename This, BOOST_PHOENIX_typename_A(N), typename Context>               \
        struct result<This(BOOST_PHOENIX_A(N), Context)>                                      \
        {                                                                                     \
            typedef std::shared_ptr<typename detail::result_of::target<A0>::type > type;    \
        };                                                                                    \
        template <BOOST_PHOENIX_typename_A(N), typename Context>                              \
        typename std::shared_ptr<typename detail::result_of::target<A0>::type >             \
        operator()(                                                                           \
            A0 const&                                                                         \
          , BOOST_PP_ENUM_SHIFTED_BINARY_PARAMS(                                              \
                N                                                                             \
              , A                                                                             \
              , const& a                                                                      \
            )                                                                                 \
          , Context const & ctx                                                               \
        ) const                                                                               \
        {                                                                                     \
            return                                                                            \
                std::make_shared<typename detail::result_of::target<A0>::type>(             \
                    BOOST_PP_REPEAT_FROM_TO(                                                  \
                        1                                                                     \
                      , N                                                                     \
                      , EVAL_a, _                                                             \
                    )                                                                         \
                );                                                                            \
        }


        BOOST_PP_REPEAT_FROM_TO(2,BOOST_PHOENIX_COMPOSITE_LIMIT,GENERATE_MAKE_SHARED_EVAL,_)

        #undef EVAL_a

        #undef GENERATE_MAKE_SHARED_EVAL


    };

    template <typename Dummy>
    struct default_actions::when<rule::make_shared, Dummy>
        : call<make_shared_eval, Dummy>
    {};

    template <typename T>
    inline
    typename expression::make_shared<detail::target<T> >::type const
    make_shared()
    {
        return
            expression::
                make_shared<detail::target<T> >::
                    make(detail::target<T>());
    }

    #define GENERATE_MAKE_SHARED(_,N,__)                                                        \
        template <typename T, BOOST_PHOENIX_typename_A(N)>                                      \
        inline                                                                                  \
        typename expression::make_shared<detail::target<T>, BOOST_PHOENIX_A(N)>::type const     \
        make_shared(BOOST_PHOENIX_A_const_ref_a(N))                                             \
        {                                                                                       \
            return                                                                              \
                expression::                                                                    \
                    make_shared<detail::target<T>, BOOST_PHOENIX_A(N)>::                        \
                        make(detail::target<T>(), BOOST_PHOENIX_a(N));                          \
        }

    BOOST_PP_REPEAT_FROM_TO(1,BOOST_PHOENIX_COMPOSITE_LIMIT,GENERATE_MAKE_SHARED,_)

    #undef GENERATE_MAKE_SHARED


}}

#endif
