# Progress Log

Running record of what's been built, in order. Newest entries at the bottom.

## Architecture

```text
RuleSet (raw config)
      |
      v
RulePreparer::prepare()
      |
      +--> StringMatcher(pattern)
      +--> Pcre2Matcher(pattern)   <- pcre2_compile() runs once, here
      |
      v
PreparedRuleSet
      |
      v
Engine::inspect(WafRequest, PreparedRuleSet)
      |
      v
Decision (Block + matchedRuleId, or Allow)
```

- `include/core/` — shared request/decision types (`WafRequest`, `Decision`).
- `include/rule/`, `src/rule/` — raw `Rule`/`RuleSet` config types, `RulePreparer` (converts raw rules into `PreparedRule`s with a real matcher object attached).
- `include/matcher/`, `src/matcher/` — `Matcher` interface, `StringMatcher` (substring search), `Pcre2Matcher` (regex, compiled once at construction).
- `include/engine/`, `src/engine/` — `Engine::inspect`, the per-request loop.

**Engine responsibility boundary** (decided): `Engine::inspect` only answers "does this request match a blocking rule?" and returns a `Decision` — it never enforces anything itself (no HTTP response, no socket handling). Enforcement is a later integration-layer concern:

```text
Web Server -> Hook Shim -> Adapter -> Engine -> Decision (Block | Allow)
```

The Adapter/Hook (not yet built, out of v1 scope — see `ROADMAP.md`) is what turns a `Block` decision into an actual HTTP 403 or dropped request, and a `Allow` decision into letting the request continue. This keeps `Engine::inspect`'s contract simple: `WafRequest + PreparedRuleSet -> Decision`, nothing more.

Build targets (`CMakeLists.txt`):
- `filtered_waf_core` — static library holding all of the above. Both the app and the tests link against this, so tests exercise the real implementation, not a copy.
- `filtered_waf_engine` — thin executable, just `src/main.cpp`, links `filtered_waf_core`.
- `filtered_waf_tests` — GoogleTest binary, links `filtered_waf_core` + `GTest::gtest_main`. Discovered by `ctest` via `gtest_discover_tests`.

## Status by component

| Component | Status |
|---|---|
| `WafRequest` / `Rule` / `RuleSet` / `RuleTarget` / `MatcherType` / `RuleAction` | Done |
| `Matcher` interface | Done |
| `StringMatcher` | Done + tested |
| `Pcre2Matcher` | Done + tested (compiles regex once in constructor, throws `std::runtime_error` on invalid pattern) |
| `PreparedRule` / `PreparedRuleSet` | Done |
| `RulePreparer` | Done + tested |
| `Engine::inspect` | Done, **not yet tested** |
| Engine/Adapter responsibility boundary | Decided — see above |
| `RuleAction::Allow` (per-rule field) | **Intentionally deferred no-op** — see below |
| `filtered_waf_core` static library split | Done |
| GoogleTest infrastructure | Done |

Test suite: 9/9 passing (`ctest --test-dir cmake-build-debug`).
- `StringMatcherTest` — 2 tests
- `Pcre2MatcherTest` — 3 tests (match, no-match, invalid-regex-throws)
- `RulePreparerTest` — 4 tests (string rule, pcre2 rule, invalid-regex propagation, multi-rule)

## Decided: Engine only returns a Decision, never enforces it

`Engine::inspect`'s job is exactly "does this request match a blocking rule?" — nothing more. It returns `Decision{ Block, matchedRuleId }` or `Decision{ Allow, -1 }` and stops there; it never sends an HTTP response, drops a connection, or does anything else that counts as enforcement. That's correct and intentional — actually *acting* on a `Block`/`Allow` decision belongs to a later Adapter/Hook integration layer (out of v1 scope, see `ROADMAP.md`), not the Engine itself. No change needed here; this is settled.

## Deferred (not fixed): `RuleAction::Allow` is a no-op

This is a distinct issue from the one above — it's about the **per-rule** `action` field a config author sets on an individual `Rule`, not about the Engine's overall `Decision` output.

`Engine::inspect` currently only special-cases `RuleAction::Block`:

```cpp
if (matched) {
    if (rule.action == RuleAction::Block) {
        return { .action = DecisionAction::Block, .matchedRuleId = rule.id };
    }
}
```

If a rule matches with `.action = RuleAction::Allow`, nothing happens — the loop just continues to the next rule, exactly as if that rule didn't exist. This is a real, still-open gap: the enum value is settable on a `Rule` but has no observable effect anywhere.

**Decision: leave it as-is for v1, deliberately.** No current rule (in `main.cpp`, or in any test) uses `RuleAction::Allow` — every rule in the codebase uses `Block`. Since it's unused, it's safe to defer without risk of silently-broken behavior in v1. This is *not* the same as "resolved" — `RuleAction::Allow` remains reserved-but-inert. Two constraints going forward:

- **Do not author a `Rule` with `.action = RuleAction::Allow` expecting it to do anything** — it won't, until this is implemented.
- Before this field is used in any real rule config (v2+), it must be given real semantics (most likely: immediately return `Allow` and stop further rule evaluation — an explicit whitelist bypass) or removed from the enum. Revisit at that point, not before.

## History

1. **Matcher abstraction introduced** — `Matcher` interface + `StringMatcher`, replacing an inline `find()` call in `Engine::inspect`.
2. **Project reorganized** — flat `include/`/`src/` split into domain folders (`core/`, `rule/`, `matcher/`, `engine/`); `main.cpp` moved to `src/main.cpp`; added `.gitignore`.
3. **`Pcre2Matcher` compile-once fix** — `pcre2_compile()` moved from `matches()` (ran every request) into the constructor, stored in a `compiledCode_` member. Constructor now throws `std::runtime_error` on invalid regex instead of silently building a dead matcher.
4. **Library split + tests introduced** — sources moved into `filtered_waf_core` static library so app and tests share one implementation; added GoogleTest via `FetchContent`; first matcher unit tests.
5. **`RulePreparer` tests added** — proves the `Rule -> PreparedRule` conversion actually builds working matchers, for both `String` and `Pcre2` types, including invalid-regex propagation.

## Next steps

1. Write `Engine::inspect` end-to-end tests (path match -> Block, body regex match -> Block, clean request -> Allow). `RuleAction::Allow` semantics are not a blocker for this — see "Deferred" above; only `RuleAction::Block` and the default-Allow path need coverage for v1.
2. Formally mark **Basic Engine v1** complete once Engine tests land — see [`ROADMAP.md`](./ROADMAP.md) for what's explicitly deferred past v1.
