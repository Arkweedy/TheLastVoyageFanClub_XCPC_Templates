using Complex = std::complex<double>;

std::array<Complex, 3> solve_cubic(double a, double b,
                                   double c, double d) {
    using ld = long double;
    assert(a != 0);

    ld A = (ld)b / a;
    ld B = (ld)c / a;
    ld C = (ld)d / a;
    ld p = B - A * A / 3;
    ld q = 2 * A * A * A / 27 - A * B / 3 + C;

    ld d1 = q * q / 4;
    ld d2 = p * p * p / 27;
    ld delta = d1 + d2;
    ld tol = 32 * std::numeric_limits<ld>::epsilon()
             * std::max<ld>(1, std::abs(d1) + std::abs(d2));
    if (std::abs(delta) <= tol) delta = 0;

    std::array<Complex, 3> ans;
    ld shift = A / 3;

    if (delta > 0) {
        ld s = std::sqrt(delta);
        ld u = std::cbrt(-q / 2 - std::copysign(s, q));
        ld v = u == 0 ? 0 : -p / (3 * u);
        ld re = -(u + v) / 2 - shift;
        ld im = std::sqrt((ld)3) * (u - v) / 2;

        ans[0] = Complex((double)(u + v - shift), 0);
        ans[1] = Complex((double)re, (double)im);
        ans[2] = Complex((double)re, (double)-im);
    } else if (delta == 0) {
        ld u = std::cbrt(-q / 2);
        ans[0] = Complex((double)(2 * u - shift), 0);
        ans[1] = ans[2] = Complex((double)(-u - shift), 0);
    } else {
        const ld pi = std::acos((ld)-1);
        ld r = 2 * std::sqrt(-p / 3);
        ld z = (-q / 2) / std::sqrt(-(p * p * p) / 27);
        ld phi = std::acos(std::clamp(z, (ld)-1, (ld)1)) / 3;

        for (int i = 0; i < 3; ++i) {
            ld x = r * std::cos(phi - 2 * pi * i / 3) - shift;
            ans[i] = Complex((double)x, 0);
        }
    }
    return ans;
}
