
This folder contains a starting playground for showing Rust capabilities. 

`main.rs` starts with the following which is a classical example of accessing a potentially invalid address after a mutable access:

```rust
fn prova() {
    let mut v = vec![10, 11];
    let vptr: & mut i32 = &mut v[1];
    println!("{}", *vptr); // error
    v.push(12);

}

fn main() {
    prova();
    println!("Hello, world!");
}
```

Try to move the `println!()` in prova after `v.push`.