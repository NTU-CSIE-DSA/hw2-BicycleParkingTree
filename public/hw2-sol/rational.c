#include <stdlib.h>

#include "rational.h"

long long gcd(long long a, long long b) {
  long long tp = a % b;
  while(tp != 0) {
    a = b;
    b = tp;
    tp = a % b;
  }
  return b;
}

long long lcm(long long a, long long b) {
  return a / gcd(a, b) * b;
}


struct rational r_new(long long p, long long q) {
  struct rational new_rational = {p, q};
  return new_rational;
}

struct rational r_from(long long p) {
  struct rational new_rational = {p, 1};
  return new_rational;
}

void r_simplify(struct rational *num) {
  if(num->q == 0) num->q = 1;
  long long GCD = gcd(num->p, num->q);
  num->p /= GCD;
  num->q /= GCD;
  if(num->q < 0) {
    num->p = -num->p;
    num->q = -num->q;
  }
}

struct rational r_add(struct rational a, struct rational b) {
  struct rational ret = {a.p, a.q};
  long long LCM = lcm(ret.q, b.q);
  ret.p *= LCM / ret.q;
  ret.q = LCM;
  b.p *= LCM / b.q;
  ret.p += b.p;
  r_simplify(&ret);
  return ret;
}

struct rational r_sub(struct rational a, struct rational b) {
  struct rational ret = {a.p, a.q};
  long long LCM = lcm(ret.q, b.q);
  ret.p *= LCM / ret.q;
  ret.q = LCM;
  b.p *= LCM / b.q;
  ret.p -= b.p;
  r_simplify(&ret);
  return ret;
}

struct rational r_mul(struct rational a, struct rational b) {
  struct rational ret = {a.p * b.p, a.q * b.q};
  r_simplify(&ret);
  return ret;
}

struct rational r_div(struct rational a, struct rational b) {
  struct rational ret = {a.p * b.q, a.q * b.p};
  r_simplify(&ret);
  return ret;
}

int r_cmp(struct rational *a, struct rational *b) {
  if (a->p == b->p && a->q == b->q) {
    return 0;
  }
  if (a->p * b->q < a->q * b->p) {
    return -1;
  }
  return 1;
}

struct rational r_abs(struct rational a) {
  struct rational ret = { .p = llabs(a.p), .q = llabs(a.q) };
  return ret;
}