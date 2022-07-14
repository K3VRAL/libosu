#include "mode/ctb/juice_stream.h"

// https://github.com/ppy/osu/blob/master/osu.Game.Rulesets.Catch/Beatmaps/CatchBeatmapProcessor.cs
// https://github.com/ppy/osu/blob/master/osu.Game.Rulesets.Catch/Objects/JuiceStream.cs
// https://github.com/ppy/osu/blob/38064165413eb919b9a32433d8be5051fc1215f7/osu.Game/Rulesets/Objects/SliderEventGenerator.cs
JuiceStream omc_juicestream_generate(HitObject hit_object) {
    // const double max_length = 100000;

    // double length = fmin(max_length, totalDistance);
    // tickDistance = Math.Clamp(tickDistance, 0, length);

    // double minDistanceFromEnd = velocity * 10;

    // yield return new SliderEventDescriptor
    // {
    //     Type = SliderEventType.Head,
    //     SpanIndex = 0,
    //     SpanStartTime = startTime,
    //     Time = startTime,
    //     PathProgress = 0,
    // };

    // if (tickDistance != 0)
    // {
    //     for (int span = 0; span < spanCount; span++)
    //     {
    //         double spanStartTime = startTime + span * spanDuration;
    //         bool reversed = span % 2 == 1;

    //         var ticks = generateTicks(span, spanStartTime, spanDuration, reversed, length, tickDistance, minDistanceFromEnd, cancellationToken);

    //         if (reversed)
    //         {
    //             // For repeat spans, ticks are returned in reverse-StartTime order, which is undesirable for some rulesets
    //             ticks = ticks.Reverse();
    //         }

    //         foreach (var e in ticks)
    //             yield return e;

    //         if (span < spanCount - 1)
    //         {
    //             yield return new SliderEventDescriptor
    //             {
    //                 Type = SliderEventType.Repeat,
    //                 SpanIndex = span,
    //                 SpanStartTime = startTime + span * spanDuration,
    //                 Time = spanStartTime + spanDuration,
    //                 PathProgress = (span + 1) % 2,
    //             };
    //         }
    //     }
    // }

    // double totalDuration = spanCount * spanDuration;
    // int finalSpanIndex = spanCount - 1;
    // double finalSpanStartTime = startTime + finalSpanIndex * spanDuration;
    // double finalSpanEndTime = Math.Max(startTime + totalDuration / 2, (finalSpanStartTime + spanDuration) - (legacyLastTickOffset ?? 0));
    // double finalProgress = (finalSpanEndTime - finalSpanStartTime) / spanDuration;

    // if (spanCount % 2 == 0) finalProgress = 1 - finalProgress;

    // yield return new SliderEventDescriptor
    // {
    //     Type = SliderEventType.LegacyLastTick,
    //     SpanIndex = finalSpanIndex,
    //     SpanStartTime = finalSpanStartTime,
    //     Time = finalSpanEndTime,
    //     PathProgress = finalProgress,
    // };

    // yield return new SliderEventDescriptor
    // {
    //     Type = SliderEventType.Tail,
    //     SpanIndex = finalSpanIndex,
    //     SpanStartTime = startTime + (spanCount - 1) * spanDuration,
    //     Time = startTime + totalDuration,
    //     PathProgress = spanCount % 2,
    // };
    JuiceStream juice_stream = {
    };
    return juice_stream;
}

void omc_juicestream_free(JuiceStream *js) {

}