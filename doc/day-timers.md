## Day 10

### Timers

I will be using 64-bit timestamps and 32-bit deltas. Minimal unit is 1 microsecond. And that is enough for today.

stamp(); -> now
diff( stamp, stamp ) -> dt
to_string( stamp );

dt s -> 0..1..2..3..4..s
chrono s(3.5) -> 0...1 (3.5s)
chronoloop s(3.5) -> 0...1 (3.5s) 0...1 (7s) ...

### Easings

normalized time (0..1)
normalized values (0..1)

sequence { wait(3.5s), circin(1.2s), expout(3.4s) };
