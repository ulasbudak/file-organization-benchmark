# Dataset

The pipeline treats its input as opaque, line-oriented text — one record per line.

- **`sample.txt`** (committed): a small, **synthetic** corpus (made-up
  `word+number` tokens) so the project runs out of the box. If `RawData.txt`
  is absent, the program automatically falls back to this file.
- **`RawData.txt`** (git-ignored, *not* published): the larger corpus used for
  the headline benchmark numbers in the top-level README. In this project it was
  a public common-password wordlist (of the kind bundled with security-research
  collections such as [SecLists](https://github.com/danielmiessler/SecLists),
  MIT licensed), used **only as sample text** — no authentication or account
  data is involved. It is intentionally kept local and excluded from the repo.

To run against your own corpus, drop any UTF-8 text file (one record per line) at
`data/RawData.txt`, or replace `data/sample.txt`.
