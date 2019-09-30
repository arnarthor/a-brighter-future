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

module WithPromise = {
  exception NumberToHigh;
  exception NumberToLow;

  let main = () => {
    randomlyFailedPromise()
    |> Js.Promise.then_(maybeNumberResult =>
         switch (maybeNumberResult) {
         | Result.Ok(number) => Js.Promise.resolve(number)
         | Result.Error(_) => Js.Promise.reject(NumberToHigh)
         }
       )
    |> Js.Promise.then_(number => addTwo(number) |> Js.Promise.resolve)
    |> Js.Promise.then_(number =>
         checkIfNumberIsBiggerThanTwoPointSeven(number) |> Js.Promise.resolve
       )
    |> Js.Promise.then_(maybeNumberResult => {
         switch (maybeNumberResult) {
         | Result.Ok(number) => logNumber(number)
         | Result.Error(_) => Js.log("No number present here :(")
         };
         Js.Promise.resolve(maybeNumberResult);
       })
    |> Js.Promise.then_(maybeNumberResult =>
         switch (maybeNumberResult) {
         | Result.Ok(number) => multiplyByFour(number) |> Js.Promise.resolve
         | Result.Error(_) => Js.Promise.reject(NumberToLow)
         }
       )
    |> Js.Promise.then_(number =>
         Js.log("final number is" ++ Js.Float.toString(number))
         |> Js.Promise.resolve
       )
    |> Js.Promise.catch(_ =>
         Js.log("We had an error, but we can't really do much about it")
         |> Js.Promise.resolve
       );
  };
};

module WithFutures = {
  let main = () => {
    FutureJs.fromPromise(randomlyFailedPromise(), _ =>
      `UnknownError("Promise failed initially")
    )
    ->Future.flatMapOk(Future.value)
    ->Future.mapOk(addTwo)
    ->Future.flatMapOk(num =>
        Future.value(checkIfNumberIsBiggerThanTwoPointSeven(num))
      )
    ->Future.tapOk(logNumber)
    ->Future.mapOk(multiplyByFour)
    ->Future.get(finalResult =>
        switch (finalResult) {
        | Result.Ok(number) =>
          Js.log("final number is" ++ Js.Float.toString(number))
        | Result.Error(`NumberToHigh) => Js.log("Number was to high")
        | Result.Error(`NumberToLow) => Js.log("Number was to low")
        | Result.Error(`UnknownError(message)) =>
          Js.log("Unknown error " ++ message)
        }
      );
  };
};

module WithLetAnything = {
  module Promise = {
    let let_ = (value, fn) =>
      Js.Promise.then_(value => fn(value) |> Js.Promise.resolve, value);
  };
  module Result = {
    include Belt.Result;
    let let_ = (value, fn) =>
      switch (value) {
      | Result.Ok(v) => fn(v)
      | _ => value
      };
  };

  let main = () => {
    let%Promise maybeResult = randomlyFailedPromise();
    let%Result maybeValue = maybeResult;
    let twoAdded = addTwo(maybeValue);
    let%Result maybeBigNumber =
      checkIfNumberIsBiggerThanTwoPointSeven(twoAdded);
    let multipliedByFour = multiplyByFour(maybeBigNumber);
    Js.log(multipliedByFour);
    Result.Ok(multipliedByFour);
  };
};
