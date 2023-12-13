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
