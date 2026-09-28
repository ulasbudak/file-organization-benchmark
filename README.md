# File Organization Benchmark

A small C project that runs a text corpus through four classic **file-organization**
techniques — preprocessing, cryptographic hashing, compression, and sequential
search — and benchmarks each representation for **file size, CPU usage, RAM, and
search time**.

It was originally built for a Data Structures / File Organization course and has
been cleaned up into a portable, reproducible project.

## Pipeline

The program (`src/main.c`) runs six stages end to end:

1. **Preprocess** — read the raw corpus, keep only `[a-z0-9]` characters, require
   each record to be ≥ 7 chars and contain at least one letter *and* one digit,
   and drop duplicates. Duplicate detection uses an in-memory hash set
   (`src/strset.c`) for **O(n)** behaviour instead of rescanning the output file.
2. **Hash** — compute the **SHA-256** of every record via OpenSSL's EVP API and
   store the hex digests.
3. **Compress** — deflate each record with **zlib**, writing a length-prefixed
   binary record format (`[original_size][compressed_size][payload]`).
4. **Decompress** — read the binary format back and reconstruct the original text.
5. **Search** — sequentially search for a fixed set of query terms across the
   **raw**, **hashed**, and **compressed** representations.
6. **Report** — print file sizes, per-representation search CPU/time, average RAM,
   and decompression time.

## Project layout

```
src/            C sources
  main.c        pipeline orchestration + benchmarks
  preprocess.c  read / clean / filter records
  strset.c      O(n) hash-set for duplicate elimination
  hash.c        SHA-256 (OpenSSL EVP)
  compression.c zlib compress / decompress + binary record I/O
  storage.c     writing records / compressed records to disk
  search.c      sequential search over each representation
  utils.c       timing, file size, cross-platform CPU & RAM metrics, paths
data/           input corpus (RawData.txt) — see data/README.md
docs/           original course report and result screenshots
output/         generated at runtime (git-ignored)
```

`utils.c` includes **cross-platform** CPU and RAM measurement with native
implementations for Windows (`GetProcessTimes` / `GetProcessMemoryInfo`),
macOS (`mach` task info), and Linux (`/proc/self/statm`).

## Build & run

Requires `gcc`/`clang`, OpenSSL, and zlib.

```bash
# macOS (OpenSSL via Homebrew) or Linux — the Makefile detects Homebrew's prefix
brew install openssl zlib   # macOS only, if needed

make           # builds ./app
./app          # runs the full pipeline
```

Out of the box the program runs against the committed **synthetic** sample
(`data/sample.txt`). To reproduce the headline numbers below, drop a larger
corpus at `data/RawData.txt` (git-ignored) — see [`data/README.md`](data/README.md).

By default paths are relative to the current directory. To run against a
different base directory:

```bash
./app /path/to/base   # reads <base>/data/RawData.txt, writes <base>/output/...
```

Generated files land under `output/` (Processed text, Hashed digests, the
compressed `.bin`, and the decompressed round-trip).

## Sample results

Run on a ~100k-record corpus → **62,260** unique records after preprocessing
(Apple Silicon, results vary by machine; the committed synthetic sample is much
smaller and just demonstrates the pipeline):

| Representation | File size | Search time |
| --- | ---: | ---: |
| Raw (processed text) | 619 KB | ~1.3 s |
| SHA-256 hashes | 4.05 MB | ~1.8 s |
| Compressed (zlib) | 2.11 MB | ~4.1 s |

Decompression of the whole corpus: ~22 ms.

### Notes / findings

- **Per-record zlib compression *inflates* the data** (~3.4× here). zlib carries
  a fixed header/overhead per compressed stream, so compressing many short
  records individually costs more than it saves. Bulk-compressing the corpus (or
  batching records) would be needed for a real size win — a useful illustration
  of why record granularity matters in file organization.
- **Hashing expands storage** too (each ~10-char record becomes a 64-char hex
  digest) but gives fixed-width records and hides the plaintext.
- **Search cost tracks per-record work**: raw string compare is cheapest;
  compressed search is slowest because every record must be inflated before it
  can be compared.

## Dataset & attribution

The repo ships a small **synthetic** corpus (`data/sample.txt`). The larger
corpus used for the headline numbers was a public password wordlist used **only
as sample text**; it is **git-ignored and not published** here. No authentication
or account data is involved. Swap in any line-oriented text file to benchmark
your own corpus — see [`data/README.md`](data/README.md).

## License

[MIT](LICENSE) © Serdar Ulaş Budak
