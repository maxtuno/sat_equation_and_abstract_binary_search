///////////////////////////////////////////////////////////////////////////////
//   copyright (complexity) 2012-2018 Oscar Riveros. all rights reserved.    //
//                           oscar.riveros@peqnp.com                         //
//                                                                           //
//   without any restriction, Oscar Riveros reserved rights, patents and     //
//  commercialization of this knowledge or derived directly from this work.  //
///////////////////////////////////////////////////////////////////////////////

// https://independent.academia.edu/oarr
// https://twitter.com/maxtuno
// https://soundcloud.com/maxtuno
// https://www.youtube.com/channel/UC6PfipcnvSkYgWFkCpjvixQ
// https://klout.com/#/maxtuno
// http://jango.com/music/Oscar+Riveros
// https://www.reverbnation.com/maxtuno

#include <iostream>
#include <vector>

using I = __int128;

enum ORDER {
    DIRECT,
    INVERSE
};

auto sat_space = [](I n, const I &size) {
    std::vector<bool> bits(I(1) << size, false);
    for (I i(0); i < bits.size(); i++) {
        bits[i] = static_cast<bool>(n % 2 == 0);
        n /= 2;
    }
    return bits;
};

auto phi = [](I n, const auto &universe) {
    auto[i, s] = std::make_pair(I(0), I(0));
    for (I i(0); i < universe.size(); i++) {
        if (n % 2) {
            s += universe[i];
        }
        n /= 2;
    }
    return s;
};

std::pair<I, I> abstract_binary_search(const std::vector<I> &universe, I t) {
    auto[complexity, i, j] = std::make_tuple(I(0), I(0), (I(1) << universe.size()) - 1);
    while (i < j) {
        I n = (i + j) / 2;
        I s = phi(n, universe);
        if (s < t) {
            i = n + 1;
        } else if (s > t) {
            j = n;
        } else {
            return std::make_pair(n, complexity);;
        }
        complexity++;
    }
    return std::make_pair(I(0), complexity);
}

std::pair<I, std::vector<I>> sat_equation(const std::vector<std::vector<I>> &cnf, const I &m, const I &n) {
    auto[sat, universe] = std::make_pair(I(0), std::vector<I>(m, I(0)));
    for (I j(0); j < m; j++) {
        I e(0);
        for (I i(0); i < n; i++) {
            e += cnf[j][n - 1 - i] > I(0) ? I(1) << i : I(0);
        }
        universe.emplace_back((I(1) << e));
        sat += universe.back();
    }
    return std::make_pair(sat, universe);
}

auto i128tos = [](I n) {
    char *s = (char *) calloc(sizeof(char), std::numeric_limits<I>::max_digits10 + 1);
    do {
        *--s = "0123456789"[(n % 10)];
        n /= 10;
    } while (n);
    return std::string(s);
};

auto print = [](const auto &bits, ORDER order) {
    if (order == ORDER::DIRECT) {
        for (auto bit = bits.begin(); bit != bits.end(); bit++) {
            std::cout << *bit;
        }
    }
    if (order == ORDER::INVERSE) {
        for (auto bit = bits.rbegin(); bit != bits.rend(); bit++) {
            std::cout << *bit;
        }
    }
    std::cout << std::endl;
};

auto report = [](const auto &cnf, const std::string &formula) {
    const auto n(cnf.back().size());
    const auto m(cnf.size());

    auto[sat, universe] = sat_equation(cnf, m, n);
    auto bits = sat_space(sat, n);
    auto[universal, complexity] = abstract_binary_search(universe, sat);

    std::cout << "EXAMPLE " << formula << std::endl;
    std::cout << std::string(185, '=') << std::endl;
    std::cout << "SAT SPACE       : ";
    print(bits, ORDER::INVERSE);

    std::cout << "UNIVERSAL       : " << i128tos(universal) << std::endl;

    std::cout << "UNIVERSAL SAPCE : ";
    print(sat_space(universal, n), ORDER::DIRECT);

    std::cout << "2^(n + m)       : " << "2^(" << n << " + " << m << ")" << std::endl;
    std::cout << "ABS COMPLEXITY  : " << i128tos(complexity) << std::endl;
    std::cout << std::string(185, '-') << std::endl;
};

void ex_a() {
    // (a&((b|c)^a->c)<->b)&~(a&((b|c)^a->c)<->b)
    // a      b      c      value
    // False  False  False  False
    // False  False  True   False
    // False  True   False  False
    // False  True   True   False
    // True   False  False  False
    // True   False  True   False
    // True   True   False  False
    // True   True   True   False

    std::vector<std::vector<I>> cnf = {{1,  2,  3},
                                       {1,  2,  -3},
                                       {1,  -2, 3},
                                       {1,  -2, -3},
                                       {-1, 2,  3},
                                       {-1, 2,  -3},
                                       {-1, -2, 3},
                                       {-1, -2, -3}};
    report(cnf, "(a&((b|c)^a->c)<->b)&~(a&((b|c)^a->c)<->b)");
}

void ex_b() {
    // (a&((b|c)^a->c)|b)&~(a&((b&c)^a->c)<->b)&d
    // a      b      c      d      value
    // False  False  False  False  False
    // False  False  False  True   False
    // False  False  True   False  False
    // False  False  True   True   False
    // False  True   False  False  False
    // False  True   False  True   True
    // False  True   True   False  False
    // False  True   True   True   True
    // True   False  False  False  False
    // True   False  False  True   False
    // True   False  True   False  False
    // True   False  True   True   True
    // True   True   False  False  False
    // True   True   False  True   True
    // True   True   True   False  False
    // True   True   True   True   False

    std::vector<std::vector<I>> cnf = {{1,  2,  3,  4},
                                       {1,  2,  3,  -4},
                                       {1,  2,  -3, 4},
                                       {1,  2,  -3, -4},
                                       {1,  -2, 3,  4},
                                       {1,  -2, -3, 4},
                                       {-1, 2,  3,  4},
                                       {-1, 2,  3,  -4},
                                       {-1, 2,  -3, 4},
                                       {-1, -2, 3,  4},
                                       {-1, -2, -3, 4},
                                       {-1, -2, -3, -4}};
    report(cnf, "(a&((b|c)^a->c)|b)&~(a&((b&c)^a->c)<->b)&d");
}

void ex_c() {
    // (a&((b|c)^a->c)|b)&~(a&((b&c)^a->c)<->b)&d<->(a&b&c|d)->e
    // a      b      c      d      e      value
    // False  False  False  False  False  False
    // False  False  False  False  True   False
    // False  False  False  True   False  True
    // False  False  False  True   True   False
    // False  False  True   False  False  False
    // False  False  True   False  True   False
    // False  False  True   True   False  True
    // False  False  True   True   True   False
    // False  True   False  False  False  False
    // False  True   False  False  True   False
    // False  True   False  True   False  False
    // False  True   False  True   True   True
    // False  True   True   False  False  False
    // False  True   True   False  True   False
    // False  True   True   True   False  False
    // False  True   True   True   True   True
    // True   False  False  False  False  False
    // True   False  False  False  True   False
    // True   False  False  True   False  True
    // True   False  False  True   True   False
    // True   False  True   False  False  False
    // True   False  True   False  True   False
    // True   False  True   True   False  False
    // True   False  True   True   True   True
    // True   True   False  False  False  False
    // True   True   False  False  True   False
    // True   True   False  True   False  False
    // True   True   False  True   True   True
    // True   True   True   False  False  True
    // True   True   True   False  True   False
    // True   True   True   True   False  True
    // True   True   True   True   True   False

    std::vector<std::vector<I>> cnf = {{1,  2,  3,  4,  5},
                                       {1,  2,  3,  4,  -5},
                                       {1,  2,  3,  -4, -5},
                                       {1,  2,  -3, 4,  5},
                                       {1,  2,  -3, 4,  -5},
                                       {1,  2,  -3, -4, -5},
                                       {1,  -2, 3,  4,  5},
                                       {1,  -2, 3,  4,  -5},
                                       {1,  -2, 3,  -4, 5},
                                       {1,  -2, -3, 4,  5},
                                       {1,  -2, -3, 4,  -5},
                                       {1,  -2, -3, -4, 5},
                                       {-1, 2,  3,  4,  5},
                                       {-1, 2,  3,  4,  -5},
                                       {-1, 2,  3,  -4, -5},
                                       {-1, 2,  -3, 4,  5},
                                       {-1, 2,  -3, 4,  -5},
                                       {-1, 2,  -3, -4, 5},
                                       {-1, -2, 3,  4,  5},
                                       {-1, -2, 3,  4,  -5},
                                       {-1, -2, 3,  -4, 5},
                                       {-1, -2, -3, 4,  -5},
                                       {-1, -2, -3, -4, -5}};
    report(cnf, "(a&((b|c)^a->c)|b)&~(a&((b&c)^a->c)<->b)&d<->(a&b&c|d)->e");
}

void ex_d() {
    // ((a&((b|c)^a->c)|b)->(a&b&c|d)&d<->(a&b&c|d)->e)&~((a&((b|c)^a->c)|b)->(a&b&c|d)&d<->(a&b&c|d)->f)
    // a      b      c      d      e      f      value
    // False  False  False  False  False  False  False
    // False  False  False  False  False  True   False
    // False  False  False  False  True   False  False
    // False  False  False  False  True   True   False
    // False  False  False  True   False  False  False
    // False  False  False  True   False  True   False
    // False  False  False  True   True   False  True
    // False  False  False  True   True   True   False
    // False  False  True   False  False  False  False
    // False  False  True   False  False  True   False
    // False  False  True   False  True   False  False
    // False  False  True   False  True   True   False
    // False  False  True   True   False  False  False
    // False  False  True   True   False  True   False
    // False  False  True   True   True   False  True
    // False  False  True   True   True   True   False
    // False  True   False  False  False  False  False
    // False  True   False  False  False  True   False
    // False  True   False  False  True   False  False
    // False  True   False  False  True   True   False
    // False  True   False  True   False  False  False
    // False  True   False  True   False  True   False
    // False  True   False  True   True   False  True
    // False  True   False  True   True   True   False
    // False  True   True   False  False  False  False
    // False  True   True   False  False  True   False
    // False  True   True   False  True   False  False
    // False  True   True   False  True   True   False
    // False  True   True   True   False  False  False
    // False  True   True   True   False  True   False
    // False  True   True   True   True   False  True
    // False  True   True   True   True   True   False
    // True   False  False  False  False  False  False
    // True   False  False  False  False  True   False
    // True   False  False  False  True   False  False
    // True   False  False  False  True   True   False
    // True   False  False  True   False  False  False
    // True   False  False  True   False  True   False
    // True   False  False  True   True   False  True
    // True   False  False  True   True   True   False
    // True   False  True   False  False  False  False
    // True   False  True   False  False  True   False
    // True   False  True   False  True   False  False
    // True   False  True   False  True   True   False
    // True   False  True   True   False  False  False
    // True   False  True   True   False  True   False
    // True   False  True   True   True   False  True
    // True   False  True   True   True   True   False
    // True   True   False  False  False  False  False
    // True   True   False  False  False  True   False
    // True   True   False  False  True   False  False
    // True   True   False  False  True   True   False
    // True   True   False  True   False  False  False
    // True   True   False  True   False  True   False
    // True   True   False  True   True   False  True
    // True   True   False  True   True   True   False
    // True   True   True   False  False  False  False
    // True   True   True   False  False  True   True
    // True   True   True   False  True   False  False
    // True   True   True   False  True   True   False
    // True   True   True   True   False  False  False
    // True   True   True   True   False  True   False
    // True   True   True   True   True   False  True
    // True   True   True   True   True   True   False

    std::vector<std::vector<I>> cnf = {{1,  2,  3,  4,  5,  6},
                                       {1,  2,  3,  4,  5,  -6},
                                       {1,  2,  3,  4,  -5, 6},
                                       {1,  2,  3,  4,  -5, -6},
                                       {1,  2,  3,  -4, 5,  6},
                                       {1,  2,  3,  -4, 5,  -6},
                                       {1,  2,  3,  -4, -5, -6},
                                       {1,  2,  -3, 4,  5,  6},
                                       {1,  2,  -3, 4,  5,  -6},
                                       {1,  2,  -3, 4,  -5, 6},
                                       {1,  2,  -3, 4,  -5, -6},
                                       {1,  2,  -3, -4, 5,  6},
                                       {1,  2,  -3, -4, 5,  -6},
                                       {1,  2,  -3, -4, -5, -6},
                                       {1,  -2, 3,  4,  5,  6},
                                       {1,  -2, 3,  4,  5,  -6},
                                       {1,  -2, 3,  4,  -5, 6},
                                       {1,  -2, 3,  4,  -5, -6},
                                       {1,  -2, 3,  -4, 5,  6},
                                       {1,  -2, 3,  -4, 5,  -6},
                                       {1,  -2, 3,  -4, -5, -6},
                                       {1,  -2, -3, 4,  5,  6},
                                       {1,  -2, -3, 4,  5,  -6},
                                       {1,  -2, -3, 4,  -5, 6},
                                       {1,  -2, -3, 4,  -5, -6},
                                       {1,  -2, -3, -4, 5,  6},
                                       {1,  -2, -3, -4, 5,  -6},
                                       {1,  -2, -3, -4, -5, -6},
                                       {-1, 2,  3,  4,  5,  6},
                                       {-1, 2,  3,  4,  5,  -6},
                                       {-1, 2,  3,  4,  -5, 6},
                                       {-1, 2,  3,  4,  -5, -6},
                                       {-1, 2,  3,  -4, 5,  6},
                                       {-1, 2,  3,  -4, 5,  -6},
                                       {-1, 2,  3,  -4, -5, -6},
                                       {-1, 2,  -3, 4,  5,  6},
                                       {-1, 2,  -3, 4,  5,  -6},
                                       {-1, 2,  -3, 4,  -5, 6},
                                       {-1, 2,  -3, 4,  -5, -6},
                                       {-1, 2,  -3, -4, 5,  6},
                                       {-1, 2,  -3, -4, 5,  -6},
                                       {-1, 2,  -3, -4, -5, -6},
                                       {-1, -2, 3,  4,  5,  6},
                                       {-1, -2, 3,  4,  5,  -6},
                                       {-1, -2, 3,  4,  -5, 6},
                                       {-1, -2, 3,  4,  -5, -6},
                                       {-1, -2, 3,  -4, 5,  6},
                                       {-1, -2, 3,  -4, 5,  -6},
                                       {-1, -2, 3,  -4, -5, -6},
                                       {-1, -2, -3, 4,  5,  6},
                                       {-1, -2, -3, 4,  -5, 6},
                                       {-1, -2, -3, 4,  -5, -6},
                                       {-1, -2, -3, -4, 5,  6},
                                       {-1, -2, -3, -4, 5,  -6},
                                       {-1, -2, -3, -4, -5, -6}};
    report(cnf, "((a&((b|c)^a->c)|b)->(a&b&c|d)&d<->(a&b&c|d)->e)&~((a&((b|c)^a->c)|b)->(a&b&c|d)&d<->(a&b&c|d)->f)");
}

void ex_e() {
    // (a|b|c|d|e|~f)&(a|b|c|d|~e|f)&(a|b|c|d|~e|~f)&(a|b|c|~d|e|f)&(a|b|c|~d|e|~f)&(a|b|c|~d|~e|~f)&(a|b|~c|d|e|f)&(a|b|~c|d|e|~f)&(a|b|~c|d|~e|f)&(a|b|~c|d|~e|~f)&(~a|~b|~c|~d|~e|~f)
    // a      b      c      d      e      f      value
    // False  False  False  False  False  False  True
    // False  False  False  False  False  True   False
    // False  False  False  False  True   False  False
    // False  False  False  False  True   True   False
    // False  False  False  True   False  False  False
    // False  False  False  True   False  True   False
    // False  False  False  True   True   False  True
    // False  False  False  True   True   True   False
    // False  False  True   False  False  False  False
    // False  False  True   False  False  True   False
    // False  False  True   False  True   False  False
    // False  False  True   False  True   True   False
    // False  False  True   True   False  False  True
    // False  False  True   True   False  True   True
    // False  False  True   True   True   False  True
    // False  False  True   True   True   True   True
    // False  True   False  False  False  False  True
    // False  True   False  False  False  True   True
    // False  True   False  False  True   False  True
    // False  True   False  False  True   True   True
    // False  True   False  True   False  False  True
    // False  True   False  True   False  True   True
    // False  True   False  True   True   False  True
    // False  True   False  True   True   True   True
    // False  True   True   False  False  False  True
    // False  True   True   False  False  True   True
    // False  True   True   False  True   False  True
    // False  True   True   False  True   True   True
    // False  True   True   True   False  False  True
    // False  True   True   True   False  True   True
    // False  True   True   True   True   False  True
    // False  True   True   True   True   True   True
    // True   False  False  False  False  False  True
    // True   False  False  False  False  True   True
    // True   False  False  False  True   False  True
    // True   False  False  False  True   True   True
    // True   False  False  True   False  False  True
    // True   False  False  True   False  True   True
    // True   False  False  True   True   False  True
    // True   False  False  True   True   True   True
    // True   False  True   False  False  False  True
    // True   False  True   False  False  True   True
    // True   False  True   False  True   False  True
    // True   False  True   False  True   True   True
    // True   False  True   True   False  False  True
    // True   False  True   True   False  True   True
    // True   False  True   True   True   False  True
    // True   False  True   True   True   True   True
    // True   True   False  False  False  False  True
    // True   True   False  False  False  True   True
    // True   True   False  False  True   False  True
    // True   True   False  False  True   True   True
    // True   True   False  True   False  False  True
    // True   True   False  True   False  True   True
    // True   True   False  True   True   False  True
    // True   True   False  True   True   True   True
    // True   True   True   False  False  False  True
    // True   True   True   False  False  True   True
    // True   True   True   False  True   False  True
    // True   True   True   False  True   True   True
    // True   True   True   True   False  False  True
    // True   True   True   True   False  True   True
    // True   True   True   True   True   False  True
    // True   True   True   True   True   True   False

    std::vector<std::vector<I>> cnf = {{1,  2,  3,  4,  5,  -6},
                                       {1,  2,  3,  4,  -5, 6},
                                       {1,  2,  3,  4,  -5, -6},
                                       {1,  2,  3,  -4, 5,  6},
                                       {1,  2,  3,  -4, 5,  -6},
                                       {1,  2,  3,  -4, -5, -6},
                                       {1,  2,  -3, 4,  5,  6},
                                       {1,  2,  -3, 4,  5,  -6},
                                       {1,  2,  -3, 4,  -5, 6},
                                       {1,  2,  -3, 4,  -5, -6},
                                       {-1, -2, -3, -4, -5, -6}};
    report(cnf, "(a|b|c|d|e|~f)&(a|b|c|d|~e|f)&(a|b|c|d|~e|~f)&(a|b|c|~d|e|f)&(a|b|c|~d|e|~f)&(a|b|c|~d|~e|~f)&(a|b|~c|d|e|f)&(a|b|~c|d|e|~f)&(a|b|~c|d|~e|f)&(a|b|~c|d|~e|~f)&(~a|~b|~c|~d|~e|~f)");
}

void ex_f() {
    // (a|b|c|d|e|~f)&(a|b|c|d|~e|f)&(a|b|c|d|~e|~f)&(a|b|~c|d|e|~f)&(a|b|~c|d|~e|f)&(a|b|~c|d|~e|~f)&(~a|~b|~c|~d|~e|~f)
    // a      b      c      d      e      f      value
    // False  False  False  False  False  False  True
    // False  False  False  False  False  True   False
    // False  False  False  False  True   False  False
    // False  False  False  False  True   True   False
    // False  False  False  True   False  False  True
    // False  False  False  True   False  True   True
    // False  False  False  True   True   False  True
    // False  False  False  True   True   True   True
    // False  False  True   False  False  False  True
    // False  False  True   False  False  True   False
    // False  False  True   False  True   False  False
    // False  False  True   False  True   True   False
    // False  False  True   True   False  False  True
    // False  False  True   True   False  True   True
    // False  False  True   True   True   False  True
    // False  False  True   True   True   True   True
    // False  True   False  False  False  False  True
    // False  True   False  False  False  True   True
    // False  True   False  False  True   False  True
    // False  True   False  False  True   True   True
    // False  True   False  True   False  False  True
    // False  True   False  True   False  True   True
    // False  True   False  True   True   False  True
    // False  True   False  True   True   True   True
    // False  True   True   False  False  False  True
    // False  True   True   False  False  True   True
    // False  True   True   False  True   False  True
    // False  True   True   False  True   True   True
    // False  True   True   True   False  False  True
    // False  True   True   True   False  True   True
    // False  True   True   True   True   False  True
    // False  True   True   True   True   True   True
    // True   False  False  False  False  False  True
    // True   False  False  False  False  True   True
    // True   False  False  False  True   False  True
    // True   False  False  False  True   True   True
    // True   False  False  True   False  False  True
    // True   False  False  True   False  True   True
    // True   False  False  True   True   False  True
    // True   False  False  True   True   True   True
    // True   False  True   False  False  False  True
    // True   False  True   False  False  True   True
    // True   False  True   False  True   False  True
    // True   False  True   False  True   True   True
    // True   False  True   True   False  False  True
    // True   False  True   True   False  True   True
    // True   False  True   True   True   False  True
    // True   False  True   True   True   True   True
    // True   True   False  False  False  False  True
    // True   True   False  False  False  True   True
    // True   True   False  False  True   False  True
    // True   True   False  False  True   True   True
    // True   True   False  True   False  False  True
    // True   True   False  True   False  True   True
    // True   True   False  True   True   False  True
    // True   True   False  True   True   True   True
    // True   True   True   False  False  False  True
    // True   True   True   False  False  True   True
    // True   True   True   False  True   False  True
    // True   True   True   False  True   True   True
    // True   True   True   True   False  False  True
    // True   True   True   True   False  True   True
    // True   True   True   True   True   False  True
    // True   True   True   True   True   True   False

    std::vector<std::vector<I>> cnf = {{1,  2,  3,  4,  5,  -6},
                                       {1,  2,  3,  4,  -5, 6},
                                       {1,  2,  3,  4,  -5, -6},
                                       {1,  2,  -3, 4,  5,  -6},
                                       {1,  2,  -3, 4,  -5, 6},
                                       {1,  2,  -3, 4,  -5, -6},
                                       {-1, -2, -3, -4, -5, -6}};
    report(cnf, "(a|b|c|d|e|~f)&(a|b|c|d|~e|f)&(a|b|c|d|~e|~f)&(a|b|~c|d|e|~f)&(a|b|~c|d|~e|f)&(a|b|~c|d|~e|~f)&(~a|~b|~c|~d|~e|~f)");
}

int main(int argc, char *argv[]) {

    ex_a();
    ex_b();
    ex_c();
    ex_d();
    ex_e();
    ex_f();

    return EXIT_SUCCESS;
}
