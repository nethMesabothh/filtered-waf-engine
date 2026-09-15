# Roadmap: Basic Engine v1 scope and beyond

## Basic Engine v1 — scope

This is the frozen scope for the first milestone. Anything not listed here is
deliberately out of scope for v1 (see "Deferred past v1" below).

```text
INPUT
  WafRequest
    method
    path
    body

RULE
  id
  target        (Path | Body)
  matcher type  (String | Pcre2)
  pattern
  action        (Block is the only meaningful value in v1;
                 RuleAction::Allow exists but is an intentional no-op
                 until v2+, see PROGRESS.md)

MATCHERS
  StringMatcher  (substring search)
  Pcre2Matcher   (regex, compiled once at construction)

PREPARATION
  convert raw Rule config into PreparedRule
  compile PCRE2 patterns once, at prepare time (not per-request)

ENGINE
  iterate prepared rules
  select target value from request (path or body)
  run the rule's matcher against it
  return Block (with matched rule id) or Allow
  returns a decision only -- never enforces it (no HTTP response,
  no connection handling); enforcement is a later Adapter/Hook concern

ERROR HANDLING
  invalid regex pattern rejected at preparation time,
  before the rule can ever be evaluated against traffic

TESTS
  matcher tests    (StringMatcher, Pcre2Matcher)
  preparer tests   (RulePreparer)
  engine tests     (end-to-end: request + rules -> Decision)
```

**Exit criteria for v1**: `Engine::inspect` has end-to-end tests covering a
String-rule block, a Pcre2-rule block, and a clean-request allow. See
[`PROGRESS.md`](./PROGRESS.md) for current status against this criterion.

`RuleAction::Allow` is *not* a v1 exit criterion — it's explicitly deferred
(see `PROGRESS.md`) as long as it stays unused by any actual rule. It must be
given real semantics or removed before v2 introduces any rule that sets it.

## Deferred past v1

These are real, expected features — just not part of the basic engine. Listed
here so scope doesn't creep into v1 by accident, and so they aren't forgotten
once v1 ships.

- Headers (inspecting request headers as a rule target)
- Query parameters (inspecting the query string separately from path/body)
- Cookies
- HTTP method rules (matching/filtering on GET/POST/etc.)
- JSON body parsing (structured inspection instead of raw body text)
- Case-insensitive matching options
- Regex flags (beyond the default PCRE2 compile options currently used)
- Rule priority (explicit ordering beyond declaration order)
- Rule groups (organizing/tagging related rules)
- SQLi/XSS classifications (labeling rules by attack category)
- Policy versioning
- Hot policy reload (updating rules without restarting)
- Atomic active-rule swapping (safely swapping a `PreparedRuleSet` while serving traffic)
- Multithreading
- Benchmarks
- PCRE2 JIT (currently disabled; see `CMakeLists.txt` PCRE2 fetch options)
- Telemetry
- Event generation
- Adapter IPC
- Manager integration

## Why freeze scope now

The core pipeline (`RuleSet -> RulePreparer -> PreparedRuleSet -> Engine ->
Decision`) is architecturally sound and tested at the matcher and preparer
layers. Adding any of the deferred items before finishing v1's exit criterion
risks building on top of an untested `Engine::inspect`. Finish and test the
small, well-defined core first.
