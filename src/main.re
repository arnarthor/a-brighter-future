open Belt;

let randomlyFailedPromise = () => {
  let random = Js.Math.random();
  Js.Promise.make((~resolve, ~reject as _) =>
    resolve(.
      if (random > 0.5) {
        Result.Error(`NumberToHigh);
      } else {
        Result.Ok(random);
      },
    )
  );
};

let addTwo = a => a +. 2.;

let checkIfNumberIsBiggerThanTwoPointSeven = a =>
  if (a > 2.7) {
    Result.Ok(a);
  } else {
    Result.Error(`NumberToLow);
  };

let logNumber = a => Js.log(a);

let multiplyByFour = a => a *. 4.;
